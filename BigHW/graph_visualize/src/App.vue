<template>
  <div class="container">
    <div class="header">
      <h1>有向图相关操作可视化设计</h1>
    </div>

    <div class="main-content">
      <!-- 控制面板 -->
      <div class="panel control-panel">
        <ControlPanel
          :nodes="nodes"
          @init-graph="handleInitGraph"
          @add-edge="handleAddEdge"
          @show-adjacency="handleShowAdjacency"
          @topology-sort="handleTopologySort"
          @critical-path="handleCriticalPath"
          @clear-results="handleClearResults"
          @load-example="handleLoadExample"
        />

        <!-- 结果展示区域：基于路由渲染对应视图 -->
        <div class="results-panel">
          <router-view v-slot="{ Component }">
            <component :is="Component" v-bind="resultsProps" />
          </router-view>
        </div>
      </div>

      <!-- 可视化面板 -->
      <div class="panel visualization-panel">
        <GraphVisualization ref="graphVisualization" :nodes="nodes" :edges="edges" />
      </div>
    </div>
  </div>
</template>

<script setup lang="ts">
import { ref, computed } from 'vue'
import { useRouter } from 'vue-router'
import ControlPanel from './components/ControlPanel.vue'
import GraphVisualization from './components/GraphVisualization.vue'
import { useGraph } from './composables/useGraph.js'
import { useTopologySort } from './composables/useTopologySort.js'
import { useCriticalPath } from './composables/useCriticalPath.js'

const router = useRouter()
const graphVisualization = ref<{ updateGraph: () => void } | null>(null)

// 最小类型声明，避免隐式 any
type NodeLike = { id: number | string; label?: string; hidden?: boolean }
type EdgeLike = { from: number | string; to: number | string; hidden?: boolean }
type EdgeInput = { from: number | string; to: number | string; weight: number }

// 使用组合式函数
const {
  nodes,
  edges,
  initializeGraph,
  addEdge,
  loadExampleGraph,
  getAdjacencyList,
  highlightCriticalPath,
} = useGraph()

const { topologySteps, topologyResult, currentStep, performTopologySort } = useTopologySort()
const { criticalPathData, calculateCriticalPath } = useCriticalPath()

const adjacencyList = ref(null)

// 事件处理函数
const handleInitGraph = (nodeCount: number) => {
  clearResults()
  initializeGraph(nodeCount)
}

const handleAddEdge = (edgeData: EdgeInput) => {
  const success = addEdge(edgeData.from, edgeData.to, edgeData.weight)
  if (!success) {
    alert('添加边失败：请检查输入或边已存在')
  }
}

const handleShowAdjacency = () => {
  adjacencyList.value = getAdjacencyList()
  router.push({ name: 'adjacency' })
}

const handleTopologySort = () => {
  if (nodes.value.length === 0) {
    alert('请先构建图！')
    return
  }

  const result = performTopologySort(nodes.value, edges.value, {
    onInit() {
      // 确保初始时全部可见
      ;(nodes.value as unknown as NodeLike[]).forEach((n: NodeLike) => {
        n.hidden = false
      })
      ;(edges.value as unknown as EdgeLike[]).forEach((e: EdgeLike) => {
        e.hidden = false
      })
      if (graphVisualization.value) graphVisualization.value.updateGraph()
    },
    onDequeue(id: number | string) {
      // 隐藏出队节点与其出边
      nodes.value = (nodes.value as unknown as NodeLike[]).map((n: NodeLike) =>
        n.id === id ? { ...n, hidden: true } : n,
      )
      edges.value = (edges.value as unknown as EdgeLike[]).map((e: EdgeLike) =>
        e.from === id ? { ...e, hidden: true } : e,
      )
      if (graphVisualization.value) graphVisualization.value.updateGraph()
    },
    onDone() {
      // 排序结束后恢复显示所有节点与边
      ;(nodes.value as unknown as NodeLike[]).forEach((n: NodeLike) => {
        n.hidden = false
      })
      ;(edges.value as unknown as EdgeLike[]).forEach((e: EdgeLike) => {
        e.hidden = false
      })
      if (graphVisualization.value) graphVisualization.value.updateGraph()
    },
  })
  if (!result.success) {
    alert(result.message)
  } else {
    router.push({ name: 'topology' })
  }
}

const handleCriticalPath = () => {
  if (topologyResult.value.length === 0) {
    alert('请先进行拓扑排序！')
    return
  }

  const result = calculateCriticalPath(nodes.value, edges.value, topologyResult.value)
  if (result.success) {
    // 高亮关键路径
    highlightCriticalPath(result.criticalEdges)
    // 触发图形更新
    if (graphVisualization.value) {
      graphVisualization.value.updateGraph()
    }
    router.push({ name: 'critical' })
  }
}

const handleClearResults = () => {
  clearResults()
}

const handleLoadExample = () => {
  clearResults()
  loadExampleGraph()
}

const clearResults = () => {
  adjacencyList.value = null
  topologySteps.value.length = 0
  topologyResult.value.length = 0
  currentStep.value = 0
  criticalPathData.value = null

  // 清空可视化数据，恢复初始（无图）
  nodes.value = []
  edges.value = []
  if (graphVisualization.value) {
    graphVisualization.value.updateGraph()
  }

  // 跳转到安全的结果页，避免渲染 critical 视图时缺少数据
  router.push({ name: 'adjacency' })
}

const resultsProps = computed(() => ({
  // 三个子视图分别需要的 props，将统一从此处按路由使用
  adjacencyList: adjacencyList.value,
  steps: topologySteps.value,
  result: topologyResult.value,
  currentStep: currentStep.value,
  criticalPathData: criticalPathData.value,
}))
</script>
