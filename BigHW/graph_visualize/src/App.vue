<template>
  <div class="container">
    <div class="header">
      <h1>🔗 有向图拓扑排序与关键路径分析系统</h1>
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

        <!-- 结果展示区域 -->
        <div class="results-panel">
          <AdjacencyList v-if="adjacencyList" :adjacency-list="adjacencyList" />

          <TopologyResult
            v-if="topologySteps.length > 0"
            :steps="topologySteps"
            :result="topologyResult"
            :current-step="currentStep"
          />

          <CriticalPathResult v-if="criticalPathData" :critical-path-data="criticalPathData" />
        </div>
      </div>

      <!-- 可视化面板 -->
      <div class="panel visualization-panel">
        <GraphVisualization ref="graphVisualization" :nodes="nodes" :edges="edges" />
      </div>
    </div>
  </div>
</template>

<script>
import { ref } from 'vue'
import ControlPanel from './components/ControlPanel.vue'
import GraphVisualization from './components/GraphVisualization.vue'
import AdjacencyList from './components/AdjacencyList.vue'
import TopologyResult from './components/TopologyResult.vue'
import CriticalPathResult from './components/CriticalPathResult.vue'
import { useGraph } from './composables/useGraph'
import { useTopologySort } from './composables/useTopologySort'
import { useCriticalPath } from './composables/useCriticalPath'

export default {
  name: 'App',
  components: {
    ControlPanel,
    GraphVisualization,
    AdjacencyList,
    TopologyResult,
    CriticalPathResult,
  },
  setup() {
    const graphVisualization = ref(null)

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
    const handleInitGraph = (nodeCount) => {
      initializeGraph(nodeCount)
      clearResults()
    }

    const handleAddEdge = (edgeData) => {
      const success = addEdge(edgeData.from, edgeData.to, edgeData.weight)
      if (!success) {
        alert('添加边失败：请检查输入或边已存在')
      }
    }

    const handleShowAdjacency = () => {
      adjacencyList.value = getAdjacencyList()
    }

    const handleTopologySort = () => {
      if (nodes.value.length === 0) {
        alert('请先构建图！')
        return
      }

      const result = performTopologySort(nodes.value, edges.value)
      if (!result.success) {
        alert(result.message)
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
      }
    }

    const handleClearResults = () => {
      clearResults()
    }

    const handleLoadExample = () => {
      loadExampleGraph()
      clearResults()
    }

    const clearResults = () => {
      adjacencyList.value = null
      topologySteps.value.length = 0
      topologyResult.value.length = 0
      currentStep.value = 0
      criticalPathData.value = null
    }

    return {
      graphVisualization,
      nodes,
      edges,
      adjacencyList,
      topologySteps,
      topologyResult,
      currentStep,
      criticalPathData,
      handleInitGraph,
      handleAddEdge,
      handleShowAdjacency,
      handleTopologySort,
      handleCriticalPath,
      handleClearResults,
      handleLoadExample,
    }
  },
}
</script>
