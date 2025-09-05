import { ref } from 'vue'

type Node = { id: number; label: string }
type Edge = { from: number; to: number; weight: number }

type Step = {
  description: string
  indegrees: Record<number, number>
  queue: number[]
  processed: number[]
  dequeued: number | null
}

type SortOptions = {
  onInit?: () => void
  onDequeue?: (id: number) => void
  onDone?: () => void
}

export function useTopologySort() {
  const topologySteps = ref<Step[]>([])
  const topologyResult = ref<string[]>([])
  const currentStep = ref(0)

  const performTopologySort = (
    nodes: Node[],
    edges: Edge[],
    options: SortOptions = {},
  ): { success: true; result: string[] } | { success: false; message: string } => {
    topologySteps.value = []
    topologyResult.value = []
    currentStep.value = 0

    // 计算入度
    const indegrees: Record<number, number> = {}
    nodes.forEach((node) => {
      indegrees[node.id] = 0
    })

    edges.forEach((edge) => {
      indegrees[edge.to] = (indegrees[edge.to] ?? 0) + 1
    })

    // 拓扑排序
    const queue: number[] = []
    const result: number[] = []
    const steps: Step[] = []

    // 找出所有入度为0的节点
    nodes.forEach((node) => {
      if ((indegrees[node.id] ?? 0) === 0) {
        queue.push(node.id)
      }
    })

    steps.push({
      description: '初始化：找出所有入度为0的节点',
      indegrees: { ...indegrees },
      queue: [...queue],
      processed: [],
      dequeued: null,
    })

    let stepCount = 1
    while (queue.length > 0) {
      const current = queue.shift() as number
      result.push(current)

      // 更新相邻节点的入度
      const updatedNodes: string[] = []
      edges.forEach((edge) => {
        if (edge.from === current) {
          indegrees[edge.to] = (indegrees[edge.to] ?? 0) - 1
          updatedNodes.push(`V${edge.to}`)
          if ((indegrees[edge.to] ?? 0) === 0) {
            queue.push(edge.to)
          }
        }
      })

      let description = `处理节点 V${current}`
      if (updatedNodes.length > 0) {
        description += `，更新节点 ${updatedNodes.join(', ')} 的入度`
      }

      steps.push({
        description: description,
        indegrees: { ...indegrees },
        queue: [...queue],
        processed: [...result],
        dequeued: current,
      })

      // 计数保留如需展示可用，但避免未使用告警
      stepCount = stepCount + 1
    }

    if (result.length !== nodes.length) {
      return {
        success: false,
        message: '图中存在环路，无法进行拓扑排序！',
      }
    }

    topologySteps.value = steps
    topologyResult.value = result.map((id) => `V${id}`)

    // 动画展示过程（带回调）
    animateTopologySort(options)

    return {
      success: true as const,
      result: topologyResult.value,
    }
  }

  const animateTopologySort = (options: SortOptions = {}) => {
    currentStep.value = 0
    let step = 0
    // 初始化回调
    if (typeof options.onInit === 'function') {
      try {
        options.onInit()
      } catch {
        /* noop */
      }
    }
    const animate = () => {
      if (step < topologySteps.value.length) {
        currentStep.value = step + 1
        const s = topologySteps.value[step]
        if (s && s.dequeued != null && typeof options.onDequeue === 'function') {
          try {
            options.onDequeue(s.dequeued)
          } catch {
            /* noop */
          }
        }
        step++
        setTimeout(animate, 1500)
      } else {
        if (typeof options.onDone === 'function') {
          try {
            options.onDone()
          } catch {
            /* noop */
          }
        }
      }
    }
    setTimeout(animate, 500)
  }

  const resetTopologySort = () => {
    topologySteps.value = []
    topologyResult.value = []
    currentStep.value = 0
  }

  const getTopologyInfo = () => {
    return {
      totalSteps: topologySteps.value.length,
      isComplete: currentStep.value >= topologySteps.value.length,
      currentStepIndex: currentStep.value - 1,
      result: topologyResult.value,
    }
  }

  const nextStep = () => {
    if (currentStep.value < topologySteps.value.length) {
      currentStep.value++
    }
  }

  const prevStep = () => {
    if (currentStep.value > 0) {
      currentStep.value--
    }
  }

  const goToStep = (stepIndex: number) => {
    if (stepIndex >= 0 && stepIndex < topologySteps.value.length) {
      currentStep.value = stepIndex + 1
    }
  }

  const validateResult = (nodes: Node[], edges: Edge[]) => {
    if (topologyResult.value.length === 0) {
      return { valid: false, message: '尚未进行拓扑排序' }
    }

    if (topologyResult.value.length !== nodes.length) {
      return { valid: false, message: '拓扑排序结果不完整' }
    }

    const position: Record<number, number> = {}
    topologyResult.value.forEach((nodeLabel, index) => {
      const nodeId = parseInt(nodeLabel.slice(1), 10)
      position[nodeId] = index
    })

    for (const edge of edges) {
      if ((position[edge.from] ?? 0) >= (position[edge.to] ?? 0)) {
        return {
          valid: false,
          message: `边 V${edge.from}→V${edge.to} 违反拓扑序要求`,
        }
      }
    }

    return { valid: true, message: '拓扑排序结果正确' }
  }

  return {
    topologySteps,
    topologyResult,
    currentStep,
    performTopologySort,
    resetTopologySort,
    getTopologyInfo,
    nextStep,
    prevStep,
    goToStep,
    validateResult,
  }
}
