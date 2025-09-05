/* eslint-disable @typescript-eslint/no-explicit-any */
/// <reference types="vite/client" />

declare module '*.vue' {
  import type { DefineComponent } from 'vue'
  const component: DefineComponent<object, object, unknown>
  export default component
}

// 为 JS 组合式函数提供最小模块声明，避免在 TS SFC 中导入时报错
// 早期为 .js 版本的组合式函数准备的声明，现已迁移为 .ts，移除以避免解析到 .js 模块

// 最小 vue-router 类型声明，避免编辑器在未完成安装时的报错
declare module 'vue-router' {
  export function useRouter(): any
  export function createRouter(...args: any[]): any
  export function createWebHashHistory(...args: any[]): any
}
