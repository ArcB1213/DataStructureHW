import { ref } from 'vue'

export function useCriticalPath() {
  const criticalPathData = ref(null)

  const calculateCriticalPath = (nodes, edges, topologyResult) => {
    criticalPathData.value = null

    try {
      // 计算Ve（事件最早发生时间）
      const ve = {}
      nodes.forEach((node) => {
        ve[node.id] = 0
      })

      // 按拓扑序列计算Ve
      topologyResult.forEach((nodeLabel) => {
        const nodeId = parseInt(nodeLabel.slice(1))
        edges.forEach((edge) => {
          if (edge.from === nodeId) {
            ve[edge.to] = Math.max(ve[edge.to], ve[nodeId] + edge.weight)
          }
        })
      })

      // 计算Vl（事件最晚发生时间）
      const vl = {}
      const maxVe = Math.max(...Object.values(ve))
      nodes.forEach((node) => {
        vl[node.id] = maxVe
      })

      // 按拓扑序列逆序计算Vl
      for (let i = topologyResult.length - 1; i >= 0; i--) {
        const nodeId = parseInt(topologyResult[i].slice(1))
        edges.forEach((edge) => {
          if (edge.to === nodeId) {
            vl[edge.from] = Math.min(vl[edge.from], vl[nodeId] - edge.weight)
          }
        })
      }

      // 计算活动的最早开始时间E和最晚开始时间L
      const activities = []
      const criticalEdges = []

      edges.forEach((edge) => {
        const earliest = ve[edge.from]
        const latest = vl[edge.to] - edge.weight
        const slack = latest - earliest

        const fromLabel = nodes.find((n) => n.id === edge.from).label
        const toLabel = nodes.find((n) => n.id === edge.to).label

        activities.push({
          edge: `${fromLabel}→${toLabel}`,
          ve: ve[edge.from],
          vl: vl[edge.to],
          earliest: earliest,
          latest: latest,
          slack: slack,
        })

        if (slack === 0) {
          criticalEdges.push(edge)
        }
      })

      // 找关键路径
      const path = findCriticalPath(criticalEdges, nodes)

      criticalPathData.value = {
        activities: activities,
        path: path,
        length: maxVe,
      }

      return {
        success: true,
        criticalEdges: criticalEdges,
      }
    } catch (error) {
      console.error('计算关键路径时出错:', error)
      return {
        success: false,
        message: '计算关键路径时出现错误',
      }
    }
  }

  const findCriticalPath = (criticalEdges, nodes) => {
    if (criticalEdges.length === 0) return []

    // 构建关键边的邻接表
    const adjList = {}
    const indegrees = {}

    criticalEdges.forEach((edge) => {
      if (!adjList[edge.from]) adjList[edge.from] = []
      adjList[edge.from].push(edge.to)

      if (!indegrees[edge.to]) indegrees[edge.to] = 0
      if (!indegrees[edge.from]) indegrees[edge.from] = 0
      indegrees[edge.to]++
    })

    // 找起始节点（入度为0）
    let start = null
    for (let node in indegrees) {
      if (indegrees[node] === 0) {
        start = parseInt(node)
        break
      }
    }

    // 构建路径
    const path = []
    let current = start

    while (current !== null && current !== undefined) {
      path.push(`V${current}`)
      if (adjList[current] && adjList[current].length > 0) {
        current = adjList[current][0]
      } else {
        break
      }
    }

    return path
  }

  return {
    criticalPathData,
    calculateCriticalPath,
  }
}
