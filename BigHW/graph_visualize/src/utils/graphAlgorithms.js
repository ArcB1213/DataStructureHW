/**
 * 图算法工具函数
 */

/**
 * 检查图是否为DAG（有向无环图）
 * @param {Array} nodes - 节点数组
 * @param {Array} edges - 边数组
 * @returns {boolean} 是否为DAG
 */
export function isDAG(nodes, edges) {
  const visited = new Set()
  const inStack = new Set()

  function dfs(nodeId) {
    if (inStack.has(nodeId)) return false // 发现环
    if (visited.has(nodeId)) return true

    visited.add(nodeId)
    inStack.add(nodeId)

    const neighbors = edges.filter((e) => e.from === nodeId).map((e) => e.to)
    for (const neighbor of neighbors) {
      if (!dfs(neighbor)) return false
    }

    inStack.delete(nodeId)
    return true
  }

  for (const node of nodes) {
    if (!visited.has(node.id) && !dfs(node.id)) {
      return false
    }
  }

  return true
}

/**
 * 计算图的强连通分量
 * @param {Array} nodes - 节点数组
 * @param {Array} edges - 边数组
 * @returns {Array} 强连通分量数组
 */
export function findStronglyConnectedComponents(nodes, edges) {
  const visited = new Set()
  const stack = []
  const components = []

  // 第一次DFS，确定结束时间顺序
  function dfs1(nodeId) {
    if (visited.has(nodeId)) return

    visited.add(nodeId)
    const neighbors = edges.filter((e) => e.from === nodeId).map((e) => e.to)

    for (const neighbor of neighbors) {
      dfs1(neighbor)
    }

    stack.push(nodeId)
  }

  // 第二次DFS，在转置图上进行
  function dfs2(nodeId, component) {
    if (visited.has(nodeId)) return

    visited.add(nodeId)
    component.push(nodeId)

    const neighbors = edges.filter((e) => e.to === nodeId).map((e) => e.from)

    for (const neighbor of neighbors) {
      dfs2(neighbor, component)
    }
  }

  // 第一次DFS
  for (const node of nodes) {
    dfs1(node.id)
  }

  visited.clear()

  // 第二次DFS
  while (stack.length > 0) {
    const nodeId = stack.pop()
    if (!visited.has(nodeId)) {
      const component = []
      dfs2(nodeId, component)
      components.push(component)
    }
  }

  return components
}

/**
 * 验证拓扑排序结果的正确性
 * @param {Array} nodes - 节点数组
 * @param {Array} edges - 边数组
 * @param {Array} topologyOrder - 拓扑排序结果
 * @returns {boolean} 是否正确
 */
export function validateTopologyOrder(nodes, edges, topologyOrder) {
  // 创建节点到位置的映射
  const positionMap = {}
  topologyOrder.forEach((nodeLabel, index) => {
    const nodeId = parseInt(nodeLabel.slice(1))
    positionMap[nodeId] = index
  })

  // 检查每条边是否满足拓扑序要求
  for (const edge of edges) {
    if (positionMap[edge.from] >= positionMap[edge.to]) {
      return false
    }
  }

  return true
}

/**
 * 计算最长路径
 * @param {Array} nodes - 节点数组
 * @param {Array} edges - 边数组
 * @param {number} startNode - 起始节点
 * @returns {Object} 最长路径信息
 */
export function findLongestPath(nodes, edges, startNode) {
  const distances = {}
  const predecessors = {}

  // 初始化距离
  nodes.forEach((node) => {
    distances[node.id] = startNode === node.id ? 0 : -Infinity
    predecessors[node.id] = null
  })

  // 使用修改后的Bellman-Ford算法（寻找最长路径）
  for (let i = 0; i < nodes.length - 1; i++) {
    for (const edge of edges) {
      if (distances[edge.from] !== -Infinity) {
        const newDistance = distances[edge.from] + edge.weight
        if (newDistance > distances[edge.to]) {
          distances[edge.to] = newDistance
          predecessors[edge.to] = edge.from
        }
      }
    }
  }

  return {
    distances,
    predecessors,
  }
}
