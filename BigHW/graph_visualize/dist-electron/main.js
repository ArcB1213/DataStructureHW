import { app as o, BrowserWindow as a } from "electron";
import t from "path";
import { fileURLToPath as c } from "url";
const f = c(import.meta.url), i = t.dirname(f), l = process.env.VITE_DEV_SERVER_URL;
function s() {
  const e = new a({
    width: 800,
    height: 600,
    webPreferences: {
      preload: t.join(i, "preload.js"),
      webSecurity: !1,
      nodeIntegration: !0,
      contextIsolation: !1
    }
  });
  if (l)
    e.loadURL(l);
  else {
    const n = t.join(i, "../dist/index.html");
    console.log("Loading file:", n), e.loadFile(n);
  }
  e.webContents.openDevTools(), e.webContents.on("did-fail-load", (n, d, r) => {
    console.error("Failed to load:", d, r);
  }), e.webContents.on("did-finish-load", () => {
    console.log("Page loaded successfully");
  });
}
o.whenReady().then(s);
o.on("window-all-closed", () => {
  process.platform !== "darwin" && o.quit();
});
o.on("activate", () => {
  a.getAllWindows().length === 0 && s();
});
