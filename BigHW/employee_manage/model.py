# --- 数据模型部分 ---
from PySide6.QtWidgets import QMessageBox

class Employee:
    """教职工类，使用ID作为唯一标识"""
    def __init__(self, id, name):
        self.id = id
        self.name = name

    def __repr__(self):
        return f"Employee(id='{self.id}', name='{self.name}')"

class Department:
    """部门类，构成组织结构的节点"""
    def __init__(self, name, parent=None):
        self.name = name
        self.parent = parent
        self.children = []  # 子部门列表
        self.manager = None  # 主管 (一个)
        self.deputy_managers = []  # 主管副职 (多个)
        self.staff = []  # 其他人员 (多个)

    def add_child(self, department):
        """添加一个子部门"""
        self.children.append(department)

    def remove_child(self, department):
        """移除一个子部门"""
        if department in self.children:
            self.children.remove(department)

    def remove_employee(self, employee_to_remove):
        """从本部门移除一个员工"""
        if self.manager and self.manager[0].id == employee_to_remove.id:
            self.manager = None
            return True

        for i, (emp, role) in enumerate(self.deputy_managers):
            if emp.id == employee_to_remove.id:
                del self.deputy_managers[i]
                return True

        for i, (emp, role) in enumerate(self.staff):
            if emp.id == employee_to_remove.id:
                del self.staff[i]
                return True
        
        return False

    def add_employee(self, employee, role):
        """根据职位添加员工，并进行规则检查"""
        # 检查员工是否已在该部门任职
        all_employees_in_dept = ([self.manager] if self.manager else []) + self.deputy_managers + self.staff
        if employee.id in [e.id for e, r in all_employees_in_dept]:
             QMessageBox.warning(None, "添加失败", f"ID为 '{employee.id}' 的员工已经在本部门任职。")
             return False

        if role == "主管":
            if self.manager:
                QMessageBox.warning(None, "添加失败", f"部门 '{self.name}' 已存在一名主管。")
                return False
            self.manager = (employee, role)
        elif role == "主管副职":
            self.deputy_managers.append((employee, role))
        else: # 其他人员
            self.staff.append((employee, role))
        return True

    def get_all_employees_with_roles(self):
        """获取本部门所有员工及其职位"""
        employees = []
        if self.manager:
            employees.append(self.manager)
        employees.extend(self.deputy_managers)
        employees.extend(self.staff)
        return employees

    def __repr__(self):
        return f"Department(name='{self.name}')"


class Organization:
    """组织结构类，管理整个树形结构和所有员工"""
    def __init__(self, name):
        self.root = Department(name)
        # 使用字典存储所有员工对象，以ID作为key，确保员工对象的唯一性
        self.employees = {}
        self._next_employee_id = 1001 # 初始员工ID

    def create_employee(self, employee_name):
        """创建一个新员工，并自动生成ID"""
        new_id = str(self._next_employee_id)
        # 理论上顺序递增不会重复，但为保险起见仍检查
        while new_id in self.employees:
            self._next_employee_id += 1
            new_id = str(self._next_employee_id)
        
        new_employee = Employee(id=new_id, name=employee_name)
        self.employees[new_id] = new_employee
        self._next_employee_id += 1
        return new_employee

    def find_employee_by_id(self, employee_id):
        """通过ID查找员工"""
        return self.employees.get(employee_id)

    def find_employee_info(self, employee_name):
        """
        在整个组织中递归查找指定姓名的员工所在的所有部门和职位。
        """
        info_by_employee = {} # Key: employee object, Value: list of roles
        
        matching_employees = [emp for emp in self.employees.values() if emp.name == employee_name]
        if not matching_employees:
            return info_by_employee

        def search_in_dept(department):
            for emp, role in department.get_all_employees_with_roles():
                if emp in matching_employees:
                    if emp not in info_by_employee:
                        info_by_employee[emp] = []
                    info_by_employee[emp].append({"department": department, "role": role})
            for child in department.children:
                search_in_dept(child)

        search_in_dept(self.root)
        # 确保即便员工尚未被分配到任何部门也返回记录（空列表）
        for emp in matching_employees:
            if emp not in info_by_employee:
                info_by_employee[emp] = []
        return info_by_employee

    def find_employee_info_by_id(self, employee_id):
        """根据员工ID返回该员工对象及其在组织内的所有任职信息列表。
        返回: (employee, infos) 或 None 当ID不存在。infos 为 [{"department": Department, "role": str}, ...]
        """
        employee = self.find_employee_by_id(employee_id)
        if not employee:
            return None

        infos = []
        def search_in_dept(department):
            for emp, role in department.get_all_employees_with_roles():
                if emp.id == employee.id:
                    infos.append({"department": department, "role": role})
            for child in department.children:
                search_in_dept(child)

        search_in_dept(self.root)
        return employee, infos