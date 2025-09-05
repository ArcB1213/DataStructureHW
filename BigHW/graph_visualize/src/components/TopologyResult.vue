<template>
  <div class="result-section">
    <h4>🔄 拓扑排序过程</h4>
    <div class="progress-bar">
      <div class="progress-fill" :style="{ width: progressWidth + '%' }"></div>
    </div>
    <div v-for="(step, index) in visibleSteps" :key="index" class="topology-step">
      <span class="step-indicator">{{ index + 1 }}</span>
      <strong>步骤 {{ index + 1 }}:</strong> {{ step.description }} <br /><small
        >入度: {{ formatIndegrees(step.indegrees) }}</small
      >
      <br /><small>队列: [{{ formatQueue(step.queue) }}]</small>
    </div>
    <div
      v-if="result.length > 0 && currentStep >= steps.length"
      style="margin-top: 15px; padding: 15px; background: white; border-radius: 8px"
    >
      <strong>🎯 拓扑排序结果: </strong>
      <span style="font-family: 'Courier New', monospace; font-size: 16px; color: #007bff">
        {{ result.join(' → ') }}
      </span>
    </div>
  </div>
</template>

<script>
export default {
  name: 'TopologyResult',
  props: {
    steps: {
      type: Array,
      required: true,
    },
    result: {
      type: Array,
      required: true,
    },
    currentStep: {
      type: Number,
      default: 0,
    },
  },
  computed: {
    visibleSteps() {
      const upto = Math.max(0, Math.min(this.currentStep, this.steps.length))
      return this.steps.slice(0, upto)
    },
    progressWidth() {
      const total = this.steps.length || 0
      if (!total) return 0
      const done = Math.max(0, Math.min(this.currentStep, total))
      return (done / total) * 100
    },
  },
  methods: {
    formatIndegrees(indegrees) {
      if (!indegrees || typeof indegrees !== 'object') return '{}'
      const parts = Object.entries(indegrees).map(([k, v]) => `V${k}: ${v}`)
      return `{ ${parts.join(', ')} }`
    },
    formatQueue(queue) {
      if (!Array.isArray(queue)) return ''
      return queue.map((id) => `V${id}`).join(', ')
    },
  },
}
</script>
