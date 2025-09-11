import sys
from PySide6.QtWidgets import QApplication
from GUI import MainWindow
from model import Department, Organization

def setup_initial_data(organization: Organization):
    """为系统预置一些初始数据，方便演示"""
    top_level_names = ["学院", "党群组织", "行政机构", "直属单位", "附属单位"]
    top_level_depts = {name: Department(name, parent=organization.root) for name in top_level_names}
    for dept in top_level_depts.values(): organization.root.add_child(dept)
    cs_college = Department("计算机科学与技术学院", parent=top_level_depts["学院"])
    top_level_depts["学院"].add_child(cs_college)
    hr_dept = Department("人力资源部", parent=top_level_depts["行政机构"])
    top_level_depts["行政机构"].add_child(hr_dept)
    cs_dept = Department("计算机科学系", parent=cs_college)
    cs_college.add_child(cs_dept)

    zhangsan = organization.create_employee("申恒涛")
    cs_dept.add_employee(zhangsan, "主管")
    hr_dept.add_employee(zhangsan, "其他人员")
    lisi = organization.create_employee("王成")
    cs_dept.add_employee(lisi, "主管副职")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    my_organization = Organization("同济大学")
    setup_initial_data(my_organization)
    main_window = MainWindow(my_organization)
    main_window.show()
    sys.exit(app.exec())

