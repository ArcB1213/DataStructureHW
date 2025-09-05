<template>
  <div class="graph-container" ref="graphContainer"></div>
</template>

<script>
import { ref, onMounted, watch, nextTick, onBeforeUnmount } from 'vue'
import { Network } from 'vis-network'

export default {
  name: 'GraphVisualization',
  props: {
    nodes: {
      type: Array,
      required: true,
    },
    edges: {
      type: Array,
      required: true,
    },
  },
  setup(props) {
    const graphContainer = ref(null)
    const network = ref(null)
    const resizeObserver = ref(null)

    const options = {
      nodes: {
        shape: 'circle',
        size: 30,
        font: {
          size: 16,
          color: '#333',
          face: 'Arial',
        },
        borderWidth: 2,
        shadow: true,
      },
      edges: {
        width: 2,
        shadow: true,
        smooth: {
          type: 'curvedCW',
          roundness: 0.1,
        },
      },
      physics: {
        enabled: true,
        stabilization: { iterations: 100 },
      },
      interaction: {
        dragNodes: true,
        dragView: true,
        zoomView: true,
      },
      autoResize: true, // 启用自动调整大小
      height: '100%', // 设置高度为100%
      width: '100%', // 设置宽度为100%
    }

    const renderGraph = () => {
      if (!graphContainer.value) return

      const data = {
        nodes: props.nodes,
        edges: props.edges,
      }

      // 销毁之前的网络实例
      if (network.value) {
        network.value.destroy()
        network.value = null
      }

      // 确保容器有明确的尺寸
      const container = graphContainer.value
      if (container.clientHeight === 0) {
        container.style.height = '400px'
      }

      network.value = new Network(container, data, options)

      // 网络稳定后调整视图
      network.value.once('stabilizationIterationsDone', () => {
        network.value.fit()
      })
    }

    const updateGraph = () => {
      nextTick(() => {
        renderGraph()
      })
    }

    // 处理容器尺寸变化
    const handleResize = () => {
      if (network.value && graphContainer.value) {
        network.value.redraw()
        network.value.fit()
      }
    }

    onMounted(() => {
      renderGraph()

      // 设置ResizeObserver来监听容器尺寸变化
      if (window.ResizeObserver && graphContainer.value) {
        resizeObserver.value = new ResizeObserver(() => {
          handleResize()
        })
        resizeObserver.value.observe(graphContainer.value)
      }

      // 备用方案：监听窗口大小变化
      window.addEventListener('resize', handleResize)
    })

    onBeforeUnmount(() => {
      if (network.value) {
        network.value.destroy()
      }
      if (resizeObserver.value) {
        resizeObserver.value.disconnect()
      }
      window.removeEventListener('resize', handleResize)
    })

    watch(
      [() => props.nodes, () => props.edges],
      () => {
        renderGraph()
      },
      { deep: true },
    )

    return {
      graphContainer,
      updateGraph,
    }
  },
}
</script>

<style scoped>
.graph-container {
  width: 100%;
  height: 600px; /* 固定一个合理高度 */
  min-height: 0; /* 避免继承到更大的最小高度 */
  overflow: hidden;
  position: relative;
}
</style>
