<template>
  <div class="result-section">
    <h4>🎯 关键路径分析</h4>
    <table class="critical-path-table">
      <thead>
        <tr>
          <th>活动</th>
          <th>Ve(i)</th>
          <th>Vl(j)</th>
          <th>E(i,j)</th>
          <th>L(i,j)</th>
          <th>L-E</th>
          <th>关键活动</th>
        </tr>
      </thead>
      <tbody>
        <tr
          v-for="activity in criticalPathData.activities"
          :key="activity.edge"
          :class="{ 'critical-activity': activity.slack === 0 }"
        >
          <td>{{ activity.edge }}</td>
          <td>{{ activity.ve }}</td>
          <td>{{ activity.vl }}</td>
          <td>{{ activity.earliest }}</td>
          <td>{{ activity.latest }}</td>
          <td>{{ activity.slack }}</td>
          <td>{{ activity.slack === 0 ? '✓' : '' }}</td>
        </tr>
      </tbody>
    </table>
    <div class="critical-path-info">
      <strong>🔥 关键路径: </strong>
      <span class="critical-path-highlight">
        {{ criticalPathData.path.join(' → ') }}
      </span>
      <br /><strong>📏 关键路径长度: </strong>{{ criticalPathData.length }}
    </div>
  </div>
</template>

<script>
export default {
  name: 'CriticalPathResult',
  props: {
    criticalPathData: {
      type: Object,
      required: true,
    },
  },
}
</script>
