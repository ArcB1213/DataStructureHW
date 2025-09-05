import { ref } from 'vue'

// 基础类型
type Node = { id: number; label: string }
type Edge = { from: number; to: number; weight: number }
type Activity = {
  edge: string
  ve: number
  vl: number
  earliest: number
  latest: number
  slack: number
}
type CriticalPathData = {
  activities: Activity[]
  path: string[]
  length: number
}

export function useCriticalPath() {
  const criticalPathData = ref<CriticalPathData | null>(null)

  const calculateCriticalPath = (
    nodes: Node[],
    edges: Edge[],
    topologyResult: string[],
  ): { success: true; criticalEdges: Edge[] } | { success: false; message: string } => {
    criticalPathData.value = null

    try {
      // 计算Ve（事件最早发生时间）
      const ve: Record<number, number> = {}
      nodes.forEach((node) => {
        ve[node.id] = 0
      })

      // 按拓扑序列计算Ve
      topologyResult.forEach((nodeLabel) => {
        const nodeId = parseInt(nodeLabel.slice(1), 10)
        edges.forEach((edge) => {
          if (edge.from === nodeId) {
            const toVal = ve[edge.to] ?? 0
            const fromVal = ve[nodeId] ?? 0
            ve[edge.to] = Math.max(toVal, fromVal + edge.weight)
          }
        })
      })

      // 计算Vl（事件最晚发生时间）
      const vl: Record<number, number> = {}
      const veValues = Object.values(ve)
      const maxVe = veValues.length > 0 ? Math.max(...veValues) : 0
      nodes.forEach((node) => {
        vl[node.id] = maxVe
      })

      // 按拓扑序列逆序计算Vl
      for (let i = topologyResult.length - 1; i >= 0; i--) {
        const nodeId = parseInt(topologyResult[i].slice(1), 10)
        edges.forEach((edge) => {
          if (edge.to === nodeId) {
            const cur = vl[edge.from] ?? maxVe
            const cand = (vl[nodeId] ?? maxVe) - edge.weight
            vl[edge.from] = Math.min(cur, cand)
          }
        })
      }

      // 计算活动的最早开始时间E和最晚开始时间L
      const activities: Activity[] = []
      const criticalEdges: Edge[] = []

      edges.forEach((edge) => {
        const earliest = ve[edge.from] ?? 0
        const latest = (vl[edge.to] ?? 0) - edge.weight
        const slack = latest - earliest

        const fromNode = nodes.find((n) => n.id === edge.from)
        const toNode = nodes.find((n) => n.id === edge.to)
        const fromLabel = fromNode?.label ?? `V${edge.from}`
        const toLabel = toNode?.label ?? `V${edge.to}`

        activities.push({
          edge: `${fromLabel}→${toLabel}`,
          ve: ve[edge.from] ?? 0,
          vl: vl[edge.to] ?? 0,
          earliest,
          latest,
          slack,
        })

        if (slack === 0) {
          criticalEdges.push(edge)
        }
      })

      // 找关键路径
      const path = findCriticalPath(criticalEdges)

      criticalPathData.value = {
        activities,
        path,
        length: maxVe,
      }

      return {
        success: true as const,
        criticalEdges,
      }
    } catch (error) {
      // 将未知错误输出
      console.error('计算关键路径时出错:', error)
      return {
        success: false as const,
        message: '计算关键路径时出现错误',
      }
    }
  }

  const findCriticalPath = (criticalEdges: Edge[]): string[] => {
    if (criticalEdges.length === 0) return []

    // 构建关键边的邻接表
    const adjList: Record<number, number[]> = {}
    const indegrees: Record<number, number> = {}

    criticalEdges.forEach((edge) => {
      if (!adjList[edge.from]) adjList[edge.from] = []
      adjList[edge.from].push(edge.to)

      if (indegrees[edge.to] == null) indegrees[edge.to] = 0
      if (indegrees[edge.from] == null) indegrees[edge.from] = 0
      indegrees[edge.to] = (indegrees[edge.to] ?? 0) + 1
    })

    // 找起始节点（入度为0）
    let start: number | null = null
    for (const key in indegrees) {
      const nodeId = Number(key)
      if (indegrees[nodeId] === 0) {
        start = nodeId
        break
      }
    }

    // 构建路径
    const path: string[] = []
    let current: number | null = start

    while (current !== null && current !== undefined) {
      path.push(`V${current}`)
      const neighbors = adjList[current]
      if (neighbors && neighbors.length > 0) {
        current = neighbors[0]
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
