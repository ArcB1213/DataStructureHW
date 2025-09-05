<template>
  <div class="graph-container" ref="graphContainer"></div>
</template>

<script lang="ts">
/* eslint-disable @typescript-eslint/no-explicit-any */
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
    const graphContainer = ref<HTMLDivElement | null>(null)
    const network = ref<Network | null>(null)
    const resizeObserver = ref<ResizeObserver | null>(null)

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

      const data: any = {
        nodes: props.nodes as any,
        edges: props.edges as any,
      }

      // 销毁之前的网络实例
      if (network.value) {
        network.value.destroy()
        network.value = null
      }

      // 确保容器有明确的尺寸
      const container = graphContainer.value as HTMLDivElement
      if (container.clientHeight === 0) {
        container.style.height = '400px'
      }

      network.value = new Network(container, data, options as any)

      // 网络稳定后调整视图
      network.value.once('stabilizationIterationsDone', () => {
        network.value!.fit()
      })
    }

    const updateGraph = () => {
      nextTick(() => {
        renderGraph()
      })
    }

    // 直接更新样式而不重建网络（用于动画）
    const applyStyleUpdates = ({
      nodes = [],
      edges = [],
    }: { nodes?: any[]; edges?: any[] } = {}) => {
      if (!network.value) return
      if (nodes && nodes.length) {
        try {
          ;(network.value as any).body.data.nodes.update(nodes)
        } catch {
          // ignore
        }
      }
      if (edges && edges.length) {
        try {
          ;(network.value as any).body.data.edges.update(edges)
        } catch {
          // ignore
        }
      }
    }

    // 颜色工具
    const hexToRgb = (hex: string) => {
      const m = hex.replace('#', '')
      const bigint = parseInt(m, 16)
      const r = (bigint >> 16) & 255
      const g = (bigint >> 8) & 255
      const b = bigint & 255
      return { r, g, b }
    }
    const hexToRgba = (hex: string, alpha = 1) => {
      const { r, g, b } = hexToRgb(hex)
      return `rgba(${r}, ${g}, ${b}, ${alpha})`
    }

    // 渐隐某个节点及其出边
    const fadeOutNode = (
      nodeId: string | number,
      options: { duration?: number; steps?: number; onDone?: () => void } = {},
    ) => {
      if (!network.value) return
      const { duration = 600, steps = 6, onDone } = options
      const interval = duration / steps
      let tick = 0

      // 找到该节点的出边（只处理 from === nodeId）
      const connected = (network.value as any).getConnectedEdges(nodeId) as Array<string | number>
      const outEdgeIds = connected.filter((eid) => {
        const e = (network.value as any).body.data.edges.get(eid)
        return e && e.from === nodeId
      })

      const step = () => {
        const alpha = Math.max(0.5, 1 - tick / steps)
        // 节点颜色
        ;(network.value as any).body.data.nodes.update({
          id: nodeId,
          color: { background: hexToRgba('#e3f2fd', alpha), border: hexToRgba('#1976d2', alpha) },
        })
        // 边颜色
        outEdgeIds.forEach((eid) => {
          ;(network.value as any).body.data.edges.update({
            id: eid,
            color: { color: hexToRgba('#666', alpha) },
            width: 2,
          })
        })
        tick++
        if (tick <= steps) {
          setTimeout(step, interval)
        } else if (onDone) {
          onDone()
        }
      }
      step()
    }

    // 恢复所有节点和边的默认样式
    const resetAllStyles = () => {
      if (!network.value) return
      const nodesDs = (network.value as any).body.data.nodes
      const edgesDs = (network.value as any).body.data.edges
      const nodeIds = nodesDs.getIds()
      const edgeIds = edgesDs.getIds()
      nodesDs.update(
        nodeIds.map((id: string | number) => ({
          id,
          color: { background: '#e3f2fd', border: '#1976d2' },
        })),
      )
      edgesDs.update(
        edgeIds.map((id: string | number) => ({ id, color: { color: '#666' }, width: 2 })),
      )
    }

    // 高亮关键路径的边（重置后再高亮）
    const highlightCriticalPath = (
      criticalEdges: Array<{ from: number | string; to: number | string }>,
    ) => {
      if (!network.value) return
      const edgesDs = (network.value as any).body.data.edges
      // 1) 重置所有边
      const allIds = edgesDs.getIds()
      edgesDs.update(
        allIds.map((id: string | number) => ({ id, color: { color: '#666' }, width: 2 })),
      )

      // 2) 高亮关键边（按 from/to 匹配）
      criticalEdges.forEach((ce) => {
        const targets = edgesDs.get({
          filter: (e: any) => e.from === ce.from && e.to === ce.to,
        })
        if (Array.isArray(targets) && targets.length > 0) {
          edgesDs.update(
            targets.map((e: any) => ({ id: e.id, color: { color: '#dc3545' }, width: 4 })),
          )
        }
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
      applyStyleUpdates,
      fadeOutNode,
      resetAllStyles,
      highlightCriticalPath,
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
