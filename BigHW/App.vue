<template>
  <div id="app-container">
    <header>
      <h1>有向图算法可视化工具</h1>
    </header>

    <main>
      <div class="control-panel">
        <h2>控制面板</h2>

        <div class="input-section">
          <label for="graph-input">图数据输入 (格式: 源 目标 权重)</label>
          <textarea
            id="graph-input"
            v-model="graphInput"
            rows="10"
            placeholder="例如:\nA B 5\nA C 3\nB D 2\nC E 6\nD F 4\nE F 1"
          ></textarea>
          <div class="button-group">
            <button @click="createGraph">1. 生成图 & 邻接链表</button>
            <button @click="startTopologicalSort" :disabled="isAnimating">2. 开始拓扑排序</button>
            <button @click="calculateCriticalPath" :disabled="isAnimating">3. 计算关键路径</button>
          </div>
        </div>

        <div class="info-section">
          <h3>消息提示:</h3>
          <p :class="['message', message.type]">{{ message.text }}</p>
        </div>
      </div>

      <div class="visualization-panel">
        <div id="graph-network"></div>
      </div>

      <div class="results-panel">
        <h2>结果展示</h2>

        <div class="result-block" v-if="adjacencyList">
          <h3>邻接链表 (Adjacency List)</h3>
          <pre>{{ formattedAdjacencyList }}</pre>
        </div>

        <div class="result-block" v-if="topoSortResult.length > 0">
          <h3>拓扑排序 (Topological Sort)</h3>
          <p>
            排序结果: <strong>{{ topoSortResult.join(' -> ') }}</strong>
          </p>
          <h4>入度实时变化:</h4>
          <table>
            <thead>
              <tr>
                <th>节点</th>
                <th>当前入度</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="node in inDegreeTable" :key="node.id">
                <td>{{ node.id }}</td>
                <td>{{ node.inDegree }}</td>
              </tr>
            </tbody>
          </table>
        </div>

        <div class="result-block" v-if="criticalPathResult.ve.size > 0">
          <h3>关键路径 (Critical Path)</h3>
          <h4>事件时间 (Events - Ve, Vl)</h4>
          <table>
            <thead>
              <tr>
                <th>事件 (节点)</th>
                <th>最早发生时间 (Ve)</th>
                <th>最晚发生时间 (Vl)</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="[node, ve] in criticalPathResult.ve.entries()" :key="node">
                <td>{{ node }}</td>
                <td>{{ ve }}</td>
                <td>{{ criticalPathResult.vl.get(node) }}</td>
              </tr>
            </tbody>
          </table>

          <h4>活动时间 (Activities - E, L, Slack)</h4>
          <table>
            <thead>
              <tr>
                <th>活动 (边)</th>
                <th>权重</th>
                <th>最早开始 (E)</th>
                <th>最晚开始 (L)</th>
                <th>时间余量 (Slack)</th>
                <th>关键活动</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="edge in criticalPathResult.activities" :key="`${edge.from}-${edge.to}`">
                <td>{{ edge.from }} → {{ edge.to }}</td>
                <td>{{ edge.weight }}</td>
                <td>{{ edge.e }}</td>
                <td>{{ edge.l }}</td>
                <td>{{ edge.slack }}</td>
                <td>{{ edge.slack === 0 ? '✔️' : '' }}</td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </main>
  </div>
</template>

<script setup>
import { ref, onMounted, computed, nextTick } from 'vue'
import { Network } from 'vis-network'
import { DataSet } from 'vis-data'

// --- Reactive State ---
const graphInput = ref(
  'A B 6\nA C 4\nA D 5\nB E 1\nC E 1\nD F 2\nE G 9\nE H 7\nF H 4\nG I 2\nH I 4',
)
const message = ref({ text: '请先生成图。', type: 'info' })
const isAnimating = ref(false)

const nodes = ref([])
const edges = ref([])
const network = ref(null)
const adjacencyList = ref(null)

const topoSortResult = ref([])
const inDegreeTable = ref([])

const criticalPathResult = ref({
  ve: new Map(),
  vl: new Map(),
  activities: [],
})

// --- Computed Properties ---
const formattedAdjacencyList = computed(() => {
  if (!adjacencyList.value) return ''
  let output = ''
  for (const [key, value] of adjacencyList.value.entries()) {
    output += `${key} -> ${value.map((v) => `${v.to}(${v.weight})`).join(' -> ')}\n`
  }
  return output
})

// --- Lifecycle Hooks ---
onMounted(() => {
  const container = document.getElementById('graph-network')
  // 使用 DataSet 初始化，避免使用 Vue 的响应式数组
  const data = {
    nodes: new DataSet([]),
    edges: new DataSet([]),
  }
  const options = {
    edges: {
      arrows: 'to',
      font: {
        align: 'top',
      },
    },
    layout: {
      hierarchical: {
        enabled: true,
        sortMethod: 'directed',
        direction: 'LR',
      },
    },
    physics: {
      enabled: false,
    },
  }
  network.value = new Network(container, data, options)
})

// --- Utility Functions ---
const sleep = (ms) => new Promise((resolve) => setTimeout(resolve, ms))

// 将数组中的对象深拷贝为普通数据，避免把 Vue 的 Proxy 传入 vis-network
const toPlainArray = (arr) => arr.map((item) => ({ ...item }))

const resetState = () => {
  adjacencyList.value = null
  topoSortResult.value = []
  inDegreeTable.value = []
  criticalPathResult.value = { ve: new Map(), vl: new Map(), activities: [] }
  message.value = { text: '请先生成图。', type: 'info' }
}

const updateNodeColor = (nodeId, color) => {
  const node = nodes.value.find((n) => n.id === nodeId)
  if (node) {
    node.color = color
    network.value.body.data.nodes.update({ id: nodeId, color })
  }
}

const updateEdgeColor = (edgeId, color) => {
  const edge = edges.value.find((e) => e.id === edgeId)
  if (edge) {
    edge.color = color
    network.value.body.data.edges.update({ id: edgeId, color, width: 3 })
  }
}

// --- Core Logic Methods ---

// 1. Create Graph and Adjacency List
const createGraph = () => {
  try {
    resetState()
    const lines = graphInput.value.trim().split('\n')
    const tempNodes = new Map()
    const tempEdges = []
    const tempAdjList = new Map()

    lines.forEach((line, index) => {
      const parts = line.trim().split(/\s+/)
      if (parts.length !== 3) {
        throw new Error(`第 ${index + 1} 行格式错误: ${line}`)
      }
      const [from, to, weightStr] = parts
      const weight = parseInt(weightStr, 10)
      if (isNaN(weight)) {
        throw new Error(`第 ${index + 1} 行权重无效: ${weightStr}`)
      }

      // Add nodes
      if (!tempNodes.has(from)) tempNodes.set(from, { id: from, label: from })
      if (!tempNodes.has(to)) tempNodes.set(to, { id: to, label: to })

      // Add edges
      tempEdges.push({
        id: `${from}-${to}`,
        from,
        to,
        label: String(weight),
        weight: weight,
        font: { size: 16 },
      })

      // Build Adjacency List
      if (!tempAdjList.has(from)) tempAdjList.set(from, [])
      tempAdjList.get(from).push({ to, weight })
    })

    // 响应式存储供页面与算法逻辑使用
    nodes.value = Array.from(tempNodes.values())
    edges.value = tempEdges
    adjacencyList.value = tempAdjList

    // 使用 DataSet 传入，避免 Proxy/数组导致的私有字段错误
    const plainNodes = toPlainArray(Array.from(tempNodes.values()))
    const plainEdges = toPlainArray(tempEdges)
    network.value.setData({ nodes: new DataSet(plainNodes), edges: new DataSet(plainEdges) })
    message.value = { text: '图和邻接链表已成功生成！', type: 'success' }
  } catch (error) {
    message.value = { text: `错误: ${error.message}`, type: 'error' }
  }
}

// 2. Topological Sort
const startTopologicalSort = async () => {
  if (nodes.value.length === 0) {
    message.value = { text: '请先生成一个图。', type: 'error' }
    return
  }
  isAnimating.value = true
  message.value = { text: '开始拓扑排序...', type: 'info' }

  // Reset previous results
  topoSortResult.value = []
  nodes.value.forEach((n) => updateNodeColor(n.id, null)) // Reset colors
  edges.value.forEach((e) =>
    network.value.body.data.edges.update({ id: e.id, color: null, width: 1 }),
  )

  const inDegree = new Map()
  nodes.value.forEach((node) => inDegree.set(node.id, 0))

  edges.value.forEach((edge) => {
    inDegree.set(edge.to, inDegree.get(edge.to) + 1)
  })

  const updateInDegreeTable = () => {
    inDegreeTable.value = Array.from(inDegree.entries()).map(([id, degree]) => ({
      id,
      inDegree: degree,
    }))
  }
  updateInDegreeTable()

  const queue = nodes.value.filter((node) => inDegree.get(node.id) === 0).map((n) => n.id)

  queue.forEach((nodeId) => updateNodeColor(nodeId, '#90EE90')) // Light green for initial queue
  await sleep(1000)

  while (queue.length > 0) {
    const u = queue.shift()
    topoSortResult.value.push(u)
    updateNodeColor(u, '#32CD32') // Green for processed
    await sleep(1000)

    const neighbors = adjacencyList.value.get(u) || []
    for (const { to } of neighbors) {
      inDegree.set(to, inDegree.get(to) - 1)
      updateInDegreeTable()
      await nextTick() // Wait for DOM to update

      updateNodeColor(to, '#FFFFE0') // Light yellow for processing neighbor
      await sleep(500)

      if (inDegree.get(to) === 0) {
        queue.push(to)
        updateNodeColor(to, '#90EE90') // Light green, ready for queue
      } else {
        updateNodeColor(to, null) // Reset color if not ready
      }
      await sleep(500)
    }
  }

  if (topoSortResult.value.length !== nodes.value.length) {
    message.value = { text: '图中存在环，无法完成拓扑排序！', type: 'error' }
  } else {
    message.value = { text: '拓扑排序完成！', type: 'success' }
  }
  isAnimating.value = false
}

// 3. Critical Path
const calculateCriticalPath = async () => {
  if (nodes.value.length === 0) {
    message.value = { text: '请先生成一个图。', type: 'error' }
    return
  }
  isAnimating.value = true
  message.value = { text: '计算关键路径中...', type: 'info' }

  // Reset colors
  nodes.value.forEach((n) => updateNodeColor(n.id, null))
  edges.value.forEach((e) =>
    network.value.body.data.edges.update({ id: e.id, color: null, width: 1 }),
  )

  // --- Step 1: Perform a silent topological sort to get the order ---
  const inDegree = new Map()
  nodes.value.forEach((node) => inDegree.set(node.id, 0))
  edges.value.forEach((edge) => inDegree.set(edge.to, inDegree.get(edge.to) + 1))

  const queue = nodes.value.filter((node) => inDegree.get(node.id) === 0).map((n) => n.id)
  const topoOrder = []

  while (queue.length > 0) {
    const u = queue.shift()
    topoOrder.push(u)
    const neighbors = adjacencyList.value.get(u) || []
    for (const { to } of neighbors) {
      inDegree.set(to, inDegree.get(to) - 1)
      if (inDegree.get(to) === 0) {
        queue.push(to)
      }
    }
  }

  if (topoOrder.length !== nodes.value.length) {
    message.value = { text: '图中存在环，无法计算关键路径！', type: 'error' }
    isAnimating.value = false
    return
  }

  // --- Step 2: Calculate Ve (Forward pass) ---
  const ve = new Map()
  nodes.value.forEach((node) => ve.set(node.id, 0))

  for (const u of topoOrder) {
    updateNodeColor(u, '#ADD8E6') // Light blue for forward pass
    await sleep(500)
    const neighbors = adjacencyList.value.get(u) || []
    for (const { to, weight } of neighbors) {
      if (ve.get(to) < ve.get(u) + weight) {
        ve.set(to, ve.get(u) + weight)
      }
    }
    updateNodeColor(u, '#87CEFA') // Deeper blue
  }

  criticalPathResult.value.ve = ve

  // --- Step 3: Calculate Vl (Backward pass) ---
  const projectDuration = Math.max(...Array.from(ve.values()))
  const vl = new Map()
  nodes.value.forEach((node) => vl.set(node.id, projectDuration))

  for (let i = topoOrder.length - 1; i >= 0; i--) {
    const u = topoOrder[i]
    updateNodeColor(u, '#FFB6C1') // Light pink for backward pass
    await sleep(500)
    const neighbors = adjacencyList.value.get(u) || []
    for (const { to, weight } of neighbors) {
      if (vl.get(u) > vl.get(to) - weight) {
        vl.set(u, vl.get(to) - weight)
      }
    }
    updateNodeColor(u, null) // Reset color after processing
  }

  criticalPathResult.value.vl = vl

  // --- Step 4: Calculate E, L, Slack and identify critical path ---
  const activities = []
  for (const edge of edges.value) {
    const { from, to, weight } = edge
    const e = ve.get(from)
    const l = vl.get(to) - weight
    const slack = l - e
    activities.push({ from, to, weight, e, l, slack })

    if (slack === 0) {
      updateNodeColor(from, '#FF4500') // OrangeRed for critical nodes
      updateNodeColor(to, '#FF4500')
      updateEdgeColor(edge.id, '#FF4500') // OrangeRed for critical edges
    }
  }

  criticalPathResult.value.activities = activities.sort((a, b) =>
    `${a.from}-${a.to}`.localeCompare(`${b.from}-${b.to}`),
  )
  message.value = { text: '关键路径计算完成！', type: 'success' }
  isAnimating.value = false
}
</script>

<style>
:root {
  --primary-color: #42b983;
  --secondary-color: #f0f8ff;
  --border-color: #ddd;
  --error-color: #d9534f;
  --success-color: #5cb85c;
  --info-color: #5bc0de;
  --bg-color: #fff;
  --text-color: #333;
}

body {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: var(--text-color);
  background-color: #f4f6f9;
  margin: 0;
  padding: 0;
}

#app-container {
  display: flex;
  flex-direction: column;
  height: 100vh;
}

header {
  background-color: var(--primary-color);
  color: white;
  padding: 1rem;
  text-align: center;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  flex-shrink: 0;
}

main {
  display: grid;
  grid-template-columns: 300px 1fr 400px;
  grid-template-rows: 1fr;
  gap: 1rem;
  padding: 1rem;
  flex-grow: 1;
  overflow: hidden;
}

.control-panel,
.visualization-panel,
.results-panel {
  background: var(--bg-color);
  border-radius: 8px;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);
  padding: 1.5rem;
  overflow-y: auto;
}

h2 {
  color: var(--primary-color);
  border-bottom: 2px solid var(--secondary-color);
  padding-bottom: 0.5rem;
  margin-top: 0;
}

h3 {
  margin-bottom: 0.5rem;
}

/* Control Panel */
.input-section label {
  display: block;
  font-weight: bold;
  margin-bottom: 0.5rem;
}

textarea#graph-input {
  width: 100%;
  box-sizing: border-box;
  border: 1px solid var(--border-color);
  border-radius: 4px;
  padding: 0.5rem;
  font-family: 'Courier New', Courier, monospace;
}

.button-group {
  margin-top: 1rem;
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

button {
  background-color: var(--primary-color);
  color: white;
  border: none;
  padding: 0.75rem;
  border-radius: 4px;
  cursor: pointer;
  font-size: 1rem;
  transition: background-color 0.3s ease;
}

button:hover:not(:disabled) {
  background-color: #36a473;
}

button:disabled {
  background-color: #ccc;
  cursor: not-allowed;
}

.info-section .message {
  padding: 0.75rem;
  border-radius: 4px;
  font-weight: bold;
}
.message.error {
  background-color: #f2dede;
  color: var(--error-color);
}
.message.success {
  background-color: #dff0d8;
  color: var(--success-color);
}
.message.info {
  background-color: #d9edf7;
  color: var(--info-color);
}

/* Visualization Panel */
#graph-network {
  width: 100%;
  height: 100%;
  min-height: 600px;
  border: 1px solid var(--border-color);
  border-radius: 4px;
}

/* Results Panel */
.results-panel .result-block {
  margin-bottom: 1.5rem;
}

pre {
  background-color: var(--secondary-color);
  padding: 1rem;
  border-radius: 4px;
  white-space: pre-wrap;
  word-wrap: break-word;
  font-family: 'Courier New', Courier, monospace;
}

table {
  width: 100%;
  border-collapse: collapse;
  margin-top: 1rem;
}
th,
td {
  border: 1px solid var(--border-color);
  padding: 0.5rem;
  text-align: center;
}
th {
  background-color: var(--secondary-color);
}
tbody tr:nth-child(even) {
  background-color: #f9f9f9;
}
</style>
