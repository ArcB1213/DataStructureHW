/* eslint-disable @typescript-eslint/no-explicit-any */
/// <reference types="vite/client" />

declare module '*.vue' {
  import type { DefineComponent } from 'vue'
  const component: DefineComponent<object, object, unknown>
  export default component
}

// 为 JS 组合式函数提供最小模块声明，避免在 TS SFC 中导入时报错
declare module './composables/useGraph.js' {
  export function useGraph(): any
}
declare module './composables/useTopologySort.js' {
  export function useTopologySort(): any
}
declare module './composables/useCriticalPath.js' {
  export function useCriticalPath(): any
}

// 最小 vue-router 类型声明，避免编辑器在未完成安装时的报错
declare module 'vue-router' {
  export function useRouter(): any
  export function createRouter(...args: any[]): any
  export function createWebHashHistory(...args: any[]): any
}
