<template>
  <div class="result-section">
    <h4>📋 邻接链表</h4>
    <div class="adjacency-list">
      <div v-for="(adj, node) in adjacencyList" :key="node" class="node-item">
        <strong>{{ node }}:</strong>
        <span v-if="adj.length === 0">∅</span>
        <span v-else>{{ adj.join(' → ') }}</span>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'AdjacencyList',
  props: {
    adjacencyList: {
      type: Object,
      required: true,
    },
  },
}
</script>
