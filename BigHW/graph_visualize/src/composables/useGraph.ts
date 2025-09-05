import { ref } from 'vue'

type Node = {
  id: number
  label: string
  color?: { background: string; border: string }
}

type Edge = {
  from: number
  to: number
  weight: number
  label?: string
  arrows?: string
  color?: { color: string }
  font?: { size: number }
  width?: number
}

export function useGraph() {
  const nodes = ref<Node[]>([])
  const edges = ref<Edge[]>([])

  const initializeGraph = (nodeCount: number) => {
    nodes.value = []
    edges.value = []

    for (let i = 0; i < nodeCount; i++) {
      nodes.value.push({
        id: i,
        label: `V${i}`,
        color: {
          background: '#e3f2fd',
          border: '#1976d2',
        },
      })
    }
  }

  const addEdge = (fromId: number, toId: number, weight: number) => {
    // 检查是否已存在相同的边
    const exists = edges.value.some((edge) => edge.from === fromId && edge.to === toId)

    if (exists) {
      return false
    }

    edges.value.push({
      from: fromId,
      to: toId,
      label: weight.toString(),
      arrows: 'to',
      color: { color: '#666' },
      font: { size: 14 },
      weight: weight,
    })

    return true
  }

  const loadExampleGraph = () => {
    initializeGraph(6)

    const exampleEdges: Array<{ from: number; to: number; weight: number }> = [
      { from: 0, to: 1, weight: 3 },
      { from: 0, to: 2, weight: 2 },
      { from: 1, to: 3, weight: 2 },
      { from: 1, to: 4, weight: 3 },
      { from: 2, to: 3, weight: 4 },
      { from: 2, to: 5, weight: 3 },
      { from: 3, to: 5, weight: 2 },
      { from: 4, to: 5, weight: 1 },
    ]

    exampleEdges.forEach((edge) => {
      addEdge(edge.from, edge.to, edge.weight)
    })
  }

  const getAdjacencyList = (): Record<string, string[]> => {
    const adjList: Record<string, string[]> = {}

    // 初始化所有节点的邻接表
    nodes.value.forEach((node) => {
      adjList[node.label] = []
    })

    // 填充邻接表
    edges.value.forEach((edge) => {
      const fromNode = nodes.value.find((n) => n.id === edge.from)
      const toNode = nodes.value.find((n) => n.id === edge.to)
      const fromLabel = fromNode?.label ?? `V${edge.from}`
      const toLabel = toNode?.label ?? `V${edge.to}`
      adjList[fromLabel].push(`${toLabel}(${edge.weight})`)
    })

    return adjList
  }

  /*  const highlightCriticalPath = (criticalEdges) => {
    edges.value.forEach((edge) => {
      edge.color = { color: '#666' }
      edge.width = 2
    })

    criticalEdges.forEach((criticalEdge) => {
      const edge = edges.value.find((e) => e.from === criticalEdge.from && e.to === criticalEdge.to)
      if (edge) {
        edge.color = { color: '#dc3545' }
        edge.width = 4
      }
    })
  }
  */

  return {
    nodes,
    edges,
    initializeGraph,
    addEdge,
    loadExampleGraph,
    getAdjacencyList,
  }
}
