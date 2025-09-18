# graph_visualize

同济大学数据库课程设计的“算法实现”作业，学号尾号为3，选题为有向图相关算法可视化。本项目使用Vue3+electron实现，Vue3完成纯前端的可视化和算法逻辑，electron用于将项目打包为exe，便于提交可执行文件。

## Recommended IDE Setup

[VSCode](https://code.visualstudio.com/).

## Type Support for `.vue` Imports in TS

TypeScript cannot handle type information for `.vue` imports by default, so we replace the `tsc` CLI with `vue-tsc` for type checking. In editors, we need [Volar](https://marketplace.visualstudio.com/items?itemName=Vue.volar) to make the TypeScript language service aware of `.vue` types.

## Customize configuration

See [Vite Configuration Reference](https://vite.dev/config/).

## Project Setup

```sh
npm install
```

### Compile and Hot-Reload for Development

```sh
npm run dev
```

### Type-Check, Compile and Minify for Production

```sh
npm run build
```

### 打包为exe

```sh
npm run electron:build
```
