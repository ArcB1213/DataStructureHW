# --- 视图/控制器部分 ---
import re
from PySide6.QtWidgets import (QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QLineEdit, QPushButton, QTreeView, QMessageBox,QLabel, QComboBox, QFormLayout, QTextEdit, QSplitter,
    QCompleter
)
from PySide6.QtGui import QStandardItemModel, QStandardItem, QFont, QAction
from PySide6.QtCore import Qt, QStringListModel
from PySide6.QtWidgets import QSizePolicy

from model import Employee, Department, Organization

class MainWindow(QMainWindow):
    def __init__(self, organization: Organization):
        super().__init__()
        self.organization = organization
        self.setWindowTitle("高校组织机构管理系统")
        self.setGeometry(100, 100, 1200, 800)
        
        self.init_ui()
        self.populate_tree()
        self._update_employee_completer()

    def init_ui(self):
        """初始化界面组件与布局"""
        # --- 菜单栏 ---
        menu_bar = self.menuBar()
        file_menu = menu_bar.addMenu("文件")
        exit_action = QAction("退出", self)
        exit_action.triggered.connect(self.close)
        file_menu.addAction(exit_action)
        help_menu = menu_bar.addMenu("帮助")
        about_action = QAction("关于", self)
        about_action.triggered.connect(self.show_about_dialog)
        help_menu.addAction(about_action)

        # --- 主体布局 ---
        main_widget = QWidget()
        self.setCentralWidget(main_widget)
        splitter = QSplitter(Qt.Horizontal)

        # --- 左侧面板 ---
        left_panel = QWidget()
        left_layout = QVBoxLayout(left_panel)
        left_layout.addWidget(QLabel("<h3>组织结构树</h3> (在此选择操作对象)"))
        self.tree_view = QTreeView()
        self.tree_model = QStandardItemModel()
        self.tree_view.setModel(self.tree_model)
        self.tree_view.setHeaderHidden(True)
        self.tree_view.setFont(QFont("Microsoft YaHei", 10))
        left_layout.addWidget(self.tree_view)
        splitter.addWidget(left_panel)

        # --- 右侧面板 ---
        right_panel = QWidget()
        right_layout = QVBoxLayout(right_panel)

        # 部门管理
        dept_form = QFormLayout()
        self.new_dept_name_input = QLineEdit()
        add_dept_button = QPushButton("添加子部门")
        add_dept_button.clicked.connect(self.add_department)
        delete_dept_button = QPushButton("删除选中部门")
        delete_dept_button.clicked.connect(self.delete_department)
        dept_form.addRow(QLabel("<h3>部门管理</h3>"))
        dept_form.addRow("新部门名称:", self.new_dept_name_input)

        dept_btn_row = QWidget()
        dept_btn_layout = QHBoxLayout(dept_btn_row)
        dept_btn_layout.setContentsMargins(0, 0, 0, 0)
        dept_btn_layout.setSpacing(8)

        add_dept_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        delete_dept_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)

        dept_btn_layout.addWidget(add_dept_button, 1)
        dept_btn_layout.addWidget(delete_dept_button, 1)

        dept_form.addRow(dept_btn_row)
        right_layout.addLayout(dept_form)

        # 人员库管理（仅创建）
        create_emp_form = QFormLayout()
        create_emp_form.addRow(QLabel("<h3>人员库</h3>"))
        self.new_emp_name_input = QLineEdit()
        self.new_emp_name_input.setPlaceholderText("输入新人员姓名，创建后再进行分配")
        create_emp_button = QPushButton("创建人员")
        create_emp_button.clicked.connect(self.create_employee)
        create_emp_form.addRow("新人员姓名:", self.new_emp_name_input)
        create_emp_form.addRow(create_emp_button)
        right_layout.addLayout(create_emp_form)

        # 人员分配管理
        emp_form = QFormLayout()
        emp_form.addRow(QLabel("<h3>人员分配</h3>"))
        self.emp_name_input = QLineEdit()
        self.emp_name_input.setPlaceholderText("输入或选择已存在人员")
        self.emp_role_combo = QComboBox()
        self.emp_role_combo.addItems(["主管", "主管副职", "其他人员"])
        assign_emp_button = QPushButton("分配到部门")
        assign_emp_button.clicked.connect(self.assign_employee)
        delete_emp_button = QPushButton("删除选中人员")
        delete_emp_button.clicked.connect(self.delete_employee)
        emp_form.addRow("人员:", self.emp_name_input)
        emp_form.addRow("职位:", self.emp_role_combo)
        emp_btn_row = QWidget()
        emp_btn_layout = QHBoxLayout(emp_btn_row)
        emp_btn_layout.setContentsMargins(0, 0, 0, 0)
        emp_btn_layout.setSpacing(8)

        assign_emp_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)
        delete_emp_button.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Fixed)

        emp_btn_layout.addWidget(assign_emp_button, 1)
        emp_btn_layout.addWidget(delete_emp_button, 1)

        emp_form.addRow(emp_btn_row)
        right_layout.addLayout(emp_form)

        # 人员查找
        search_layout = QVBoxLayout()
        search_layout.addWidget(QLabel("<h3>人员信息查找</h3>"))
        search_hbox = QHBoxLayout()
        self.search_name_input = QLineEdit()
        self.search_name_input.setPlaceholderText("输入人员姓名（或从补全中选择“姓名 (ID: …)”）进行查询")
        search_name_button = QPushButton("查找人员")
        search_name_button.clicked.connect(self.search_employee)
        search_hbox.addWidget(self.search_name_input)
        search_hbox.addWidget(search_name_button)
        search_layout.addLayout(search_hbox)
        self.search_result_text = QTextEdit()
        self.search_result_text.setReadOnly(True)
        self.search_result_text.setFont(QFont("SimSun", 11))
        search_layout.addWidget(self.search_result_text)
        right_layout.addLayout(search_layout)

        right_panel.setLayout(right_layout)
        splitter.addWidget(right_panel)
        splitter.setSizes([450, 750])
        main_layout = QHBoxLayout(main_widget)
        main_layout.addWidget(splitter)

        # --- 自动补全设置 ---
        self.employee_completer = QCompleter(self)
        self.employee_completer.setCaseSensitivity(Qt.CaseInsensitive)
        self.employee_completer.setFilterMode(Qt.MatchContains)
        self.emp_name_input.setCompleter(self.employee_completer)
        self.search_name_input.setCompleter(self.employee_completer)
        self.employee_completer.activated.connect(self.on_completer_activated)


    def _update_employee_completer(self):
        """更新员工自动补全列表"""
        employee_list = sorted([f"{emp.name} (ID: {emp.id})" for emp in self.organization.employees.values()])
        model = QStringListModel(employee_list)
        self.employee_completer.setModel(model)
        
    def on_completer_activated(self, text):
        """当用户在自动补全列表中选择一项时触发，实现快速搜索"""
        if self.search_name_input.hasFocus():
            self.search_name_input.setText(text)
            self.search_employee()

    def populate_tree(self, parent_item=None, department=None):
        """递归填充组织结构树"""
        if parent_item is None:
            self.tree_model.clear()
            parent_item = self.tree_model.invisibleRootItem()
            department = self.organization.root

        item = QStandardItem(department.name)
        item.setData(department, Qt.UserRole)
        item.setEditable(False)
        parent_item.appendRow(item)

        for emp, role in sorted(department.get_all_employees_with_roles(), key=lambda x: x[0].id):
            info_text = f"  └ {role}: {emp.name} (ID: {emp.id})"
            emp_item = QStandardItem(info_text)
            emp_item.setEditable(False)
            emp_item.setData((emp, department), Qt.UserRole)
            item.appendRow(emp_item)
        
        for child_dept in department.children:
            self.populate_tree(item, child_dept)
            
        if parent_item == self.tree_model.invisibleRootItem():
            self.tree_view.expandAll()

    def get_selected_department(self):
        """获取当前在QTreeView中选中的部门对象"""
        selected_indexes = self.tree_view.selectedIndexes()
        if not selected_indexes: return None
        selected_item = self.tree_model.itemFromIndex(selected_indexes[0])
        if not isinstance(selected_item.data(Qt.UserRole), Department):
            selected_item = selected_item.parent()
        if selected_item: return selected_item.data(Qt.UserRole)
        return None

    def add_department(self):
        parent_dept = self.get_selected_department()
        if not parent_dept:
            QMessageBox.warning(self, "操作失败", "请先在左侧的组织树中选择一个上级部门。")
            return
        dept_name = self.new_dept_name_input.text().strip()
        if not dept_name:
            QMessageBox.warning(self, "输入错误", "部门名称不能为空。")
            return
        if any(child.name == dept_name for child in parent_dept.children):
            QMessageBox.warning(self, "添加失败", f"部门 '{parent_dept.name}' 下已存在名为 '{dept_name}' 的子部门。")
            return
        new_dept = Department(name=dept_name, parent=parent_dept)
        parent_dept.add_child(new_dept)
        self.populate_tree()
        self.new_dept_name_input.clear()
        QMessageBox.information(self, "成功", f"部门 '{dept_name}' 已成功添加到 '{parent_dept.name}' 下。")

    def delete_department(self):
        dept_to_delete = self.get_selected_department()
        if not dept_to_delete:
            QMessageBox.warning(self, "操作失败", "请先选择要删除的部门。")
            return
        if dept_to_delete == self.organization.root:
            QMessageBox.critical(self, "错误", "不能删除根组织节点！")
            return
        reply = QMessageBox.question(self, "确认删除", f"确定要删除部门 '{dept_to_delete.name}' 吗？\n这将同时删除其所有子部门！",
                                     QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply == QMessageBox.Yes:
            dept_to_delete.parent.remove_child(dept_to_delete)
            self.populate_tree()
            QMessageBox.information(self, "成功", f"部门 '{dept_to_delete.name}' 已被删除。")

    def delete_employee(self):
        selected_indexes = self.tree_view.selectedIndexes()
        if not selected_indexes:
            QMessageBox.warning(self, "操作失败", "请先在左侧的组织树中选择一名人员。")
            return
        selected_item = self.tree_model.itemFromIndex(selected_indexes[0])
        item_data = selected_item.data(Qt.UserRole)
        if not isinstance(item_data, tuple) or not isinstance(item_data[0], Employee):
            QMessageBox.warning(self, "操作失败", "请直接选择要删除的人员节点，而不是部门。")
            return
        employee_to_delete, in_department = item_data
        reply = QMessageBox.question(self, "确认删除", f"确定要从部门 '{in_department.name}' 中移除\n员工: {employee_to_delete.name} (ID: {employee_to_delete.id}) 吗？",
                                     QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply == QMessageBox.Yes:
            if in_department.remove_employee(employee_to_delete):
                self.populate_tree()
                QMessageBox.information(self, "成功", f"已从 '{in_department.name}' 移除员工 '{employee_to_delete.name}'。")
            else:
                QMessageBox.critical(self, "错误", "删除员工时发生未知错误。")

    def create_employee(self):
        """仅创建人员进入全局人员库，不做部门分配"""
        name = self.new_emp_name_input.text().strip()
        if not name:
            QMessageBox.warning(self, "输入错误", "姓名不能为空。")
            return
        # 允许重名：不再阻止同名人员创建，唯一识别依靠自动生成的ID
        new_emp = self.organization.create_employee(name)
        self._update_employee_completer()
        self.new_emp_name_input.clear()
        QMessageBox.information(self, "成功", f"已创建人员 '{new_emp.name}' (ID: {new_emp.id})。支持重名，操作时请使用含ID格式。")

    def assign_employee(self):
        """将已存在人员（需带ID）分配到选中部门"""
        selected_dept = self.get_selected_department()
        if not selected_dept:
            QMessageBox.warning(self, "操作失败", "请先在左侧选择要分配人员的部门。")
            return
        input_text = self.emp_name_input.text().strip()
        if not input_text:
            QMessageBox.warning(self, "输入错误", "请选择或输入人员（需包含ID）。")
            return
        emp_role = self.emp_role_combo.currentText()
        match = re.match(r".+ \(ID: (.+)\)", input_text)
        if not match:
            QMessageBox.warning(self, "格式错误", "请从自动补全中选择包含ID的人员，或先在人员库创建该人员。")
            return
        emp_id = match.group(1)
        employee = self.organization.find_employee_by_id(emp_id)
        if not employee:
            QMessageBox.warning(self, "未找到", f"ID为 {emp_id} 的人员不存在，请先创建。")
            return
        if selected_dept.add_employee(employee, emp_role):
            self.populate_tree()
            self.emp_name_input.clear()
            QMessageBox.information(self, "成功", f"已将 '{employee.name}' (ID: {employee.id}) 分配到 '{selected_dept.name}'，职位 '{emp_role}'。")

    def search_employee(self):
        input_text = self.search_name_input.text().strip()
        if not input_text:
            QMessageBox.warning(self, "输入错误", "请输入要查找的人员姓名，或从补全中选择包含ID的条目。")
            return

        # 如果从补全选择了“姓名 (ID: xxx)”，直接按ID查询
        id_match = re.match(r".*\(ID: ([^)]+)\)$", input_text)
        if id_match:
            emp_id = id_match.group(1).strip()
            result = self.organization.find_employee_info_by_id(emp_id)
            if not result:
                self.search_result_text.setText(f"未找到ID为 '{emp_id}' 的教职工。")
                return
            employee, infos = result
            self._render_employee_search_result(employee, infos)
            return

        # 否则按姓名精确匹配，保留重名，要求用户从补全中选中具体人员
        name = input_text
        matches = [emp for emp in self.organization.employees.values() if emp.name == name]
        if not matches:
            self.search_result_text.setText(f"未找到姓名为 '{name}' 的教职工。")
            return
        if len(matches) == 1:
            employee = matches[0]
            _, infos = self.organization.find_employee_info_by_id(employee.id)
            self._render_employee_search_result(employee, infos)
            return

        # 多个同名：提示用户通过下拉补全选择具体人员
        lines = [f"存在重名 {len(matches)} 人，请从输入框补全列表中选择具体人员：", "-" * 20]
        for emp in sorted(matches, key=lambda e: e.id):
            lines.append(f"{emp.name} (ID: {emp.id})")
        self.search_result_text.setText("\n".join(lines))
        self.search_name_input.setFocus()

    def _render_employee_search_result(self, employee, infos):
        """按统一样式渲染单个员工的查询结果"""
        result_lines = [f"员工: {employee.name} (ID: {employee.id})", "-" * 20]
        if not infos:
            result_lines.append("尚未分配到任何部门")
        else:
            for info in infos:
                path = []
                dept = info['department']
                while dept.parent:
                    path.insert(0, dept.name)
                    dept = dept.parent
                result_lines.append(f"所属部门: {' -> '.join(path)}")
                result_lines.append(f"职位: {info['role']}")
                result_lines.append("-" * 50)
        self.search_result_text.setText("\n".join(result_lines))

    def show_about_dialog(self):
        QMessageBox.about(self, "关于", "高校组织机构管理系统 V1.3\n\n数据结构课程设计\n作者: 白佳煜\n")