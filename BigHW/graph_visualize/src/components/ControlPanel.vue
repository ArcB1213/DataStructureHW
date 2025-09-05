<template>
  <div>
    <div class="input-group">
      <label>📊 图的构建</label>
      <div class="input-row">
        <input v-model.number="nodeCount" type="number" placeholder="节点数量" min="2" max="20" />
        <button @click="initGraph" class="btn btn-primary">初始化图</button>
      </div>
    </div>

    <div class="input-group" v-if="nodes.length > 0">
      <label>➕ 添加边</label>
      <div class="input-row">
        <select v-model="fromNode">
          <option value="">起点</option>
          <option v-for="node in nodes" :key="node.id" :value="node.id">
            {{ node.label }}
          </option>
        </select>
        <select v-model="toNode">
          <option value="">终点</option>
          <option v-for="node in nodes" :key="node.id" :value="node.id">
            {{ node.label }}
          </option>
        </select>
      </div>
      <div class="input-row">
        <input v-model.number="edgeWeight" type="number" placeholder="权重" min="1" max="100" />
        <button @click="addEdge" class="btn btn-secondary">添加边</button>
      </div>
    </div>

    <div class="input-group">
      <label>📋 操作菜单</label>
      <div class="input-row">
        <button @click="$emit('show-adjacency')" class="btn btn-success">显示邻接表</button>
        <button @click="$emit('topology-sort')" class="btn btn-warning">拓扑排序</button>
      </div>
      <div class="input-row">
        <button @click="$emit('critical-path')" class="btn btn-primary">关键路径</button>
        <button @click="$emit('clear-results')" class="btn btn-secondary">清除结果</button>
      </div>
    </div>

    <div class="input-group">
      <label>预设示例</label>
      <button @click="$emit('load-example')" class="btn btn-secondary" style="width: 100%">
        加载示例图
      </button>
    </div>
  </div>
</template>

<script>
import { ref } from 'vue'

export default {
  name: 'ControlPanel',
  props: {
    nodes: {
      type: Array,
      required: true,
    },
  },
  emits: [
    'init-graph',
    'add-edge',
    'show-adjacency',
    'topology-sort',
    'critical-path',
    'clear-results',
    'load-example',
  ],
  setup(props, { emit }) {
    const nodeCount = ref(6)
    const fromNode = ref('')
    const toNode = ref('')
    const edgeWeight = ref(1)

    const initGraph = () => {
      emit('init-graph', nodeCount.value)
    }

    const addEdge = () => {
      if (fromNode.value === '' || toNode.value === '' || fromNode.value === toNode.value) {
        alert('请选择有效的起点和终点！')
        return
      }

      emit('add-edge', {
        from: fromNode.value,
        to: toNode.value,
        weight: edgeWeight.value,
      })

      // 重置输入
      fromNode.value = ''
      toNode.value = ''
      edgeWeight.value = 1
    }

    return {
      nodeCount,
      fromNode,
      toNode,
      edgeWeight,
      initGraph,
      addEdge,
    }
  },
}
</script>
