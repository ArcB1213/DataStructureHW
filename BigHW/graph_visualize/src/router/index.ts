import { createRouter, createWebHashHistory } from 'vue-router'

// 路由直接使用现有组件作为视图，具体 props 由父级 App.vue 通过 router-view 传入
import AdjacencyList from '@/components/AdjacencyList.vue'
import TopologyResult from '@/components/TopologyResult.vue'
import CriticalPathResult from '@/components/CriticalPathResult.vue'

const routes = [
  { path: '/', redirect: '/adjacency' },
  { path: '/adjacency', name: 'adjacency', component: AdjacencyList },
  { path: '/topology', name: 'topology', component: TopologyResult },
  { path: '/critical', name: 'critical', component: CriticalPathResult },
]

const router = createRouter({
  history: createWebHashHistory(),
  routes,
})

export default router
