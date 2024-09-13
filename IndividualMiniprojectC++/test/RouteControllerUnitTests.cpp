// Copyright (c) 2024 Annie Xu @ Columbia University

#include <gtest/gtest.h>
#include "RouteController.h"
#include "MyApp.h"
#include <gtest/gtest.h>
#include "RouteController.h"
#include "MyApp.h"
#include "crow.h"

class RouteControllerUnitTests : public ::testing::Test {
protected:
    static RouteController routeController;
    static crow::SimpleApp app;

public:
    static void SetUpTestSuite() {
        MyApp::run("run");

        app.signal_clear();

        routeController.initRoutes(app);
        routeController.setDatabase(MyApp::getDatabase());
    }

    static void TearDownTestSuite() {
        MyApp::onTermination();
    }

    void SetUp() override {
        req = crow::request();
        res = crow::response();
    }

    crow::request req;
    crow::response res;
};

RouteController RouteControllerUnitTests::routeController;
crow::SimpleApp RouteControllerUnitTests::app;

TEST_F(RouteControllerUnitTests, RetrieveDepartmentTest) {

    const std::string phys = R"(PHYS 1001: 
Instructor: Szabolcs Marka; Location: 301 PUP; Time: 2:40-3:55
PHYS 1221: 
Instructor: James G. Mccann; Location: 301 PUP; Time: 4:10-5:25
PHYS 1520: 
Instructor: Victor G. Moffat; Location: 630 MUDD; Time: 4:10-5:25
PHYS 2000: 
Instructor: Frank E. L. Banta; Location: 402 CHANDLER; Time: 1:10-3:40
PHYS 3801: 
Instructor: Katherine M. McMahon; Location: 603 MUDD; Time: 4:10-5:25
PHYS 4205: 
Instructor: Michael P. Larkin; Location: 309 HAV; Time: 6:10-9:50
)";
    // Department found
    req.url_params = crow::query_string{"?deptCode=PHYS"};
    routeController.retrieveDepartment(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, phys);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND"};
    routeController.retrieveDepartment(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
}

TEST_F(RouteControllerUnitTests, RetrieveCourseTest) {

    // Course found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221"};
    routeController.retrieveCourse(req, res);
    EXPECT_EQ(res.code, 200);

    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());

    std::string expectedBody = courseIt->second->display();
    EXPECT_EQ(res.body, expectedBody);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221"};
    routeController.retrieveCourse(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Department found but course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000"};
    routeController.retrieveCourse(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}

TEST_F(RouteControllerUnitTests, IsCourseFullTest) {
    // Course is full
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1520"};
    routeController.isCourseFull(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "true");
    res = crow::response();

    // Course not full
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221"};
    routeController.isCourseFull(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "false");
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221"};
    routeController.isCourseFull(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000"};
    routeController.isCourseFull(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}


TEST_F(RouteControllerUnitTests, GetMajorCountFromDeptTest) {
    // Department found
    req.url_params = crow::query_string{"?deptCode=PHYS"};
    routeController.getMajorCountFromDept(req, res);

    EXPECT_EQ(res.code, 200);
    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    int majorCount = deptIt->second.getNumberOfMajors();
    std::string expectedBody = "There are: " + std::to_string(majorCount) + " majors in the department";

    EXPECT_EQ(res.body, expectedBody);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND"};
    routeController.getMajorCountFromDept(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
}

TEST_F(RouteControllerUnitTests, IdentifyDeptChairTest) {
    // Department found
    req.url_params = crow::query_string{"?deptCode=PHYS"};
    routeController.identifyDeptChair(req, res);

    EXPECT_EQ(res.code, 200);
    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    std::string chairName = deptIt->second.getDepartmentChair();
    std::string expectedBody = chairName + " is the department chair.";
    EXPECT_EQ(res.body, expectedBody);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND"};
    routeController.identifyDeptChair(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
}

TEST_F(RouteControllerUnitTests, FindCourseLocationTest) {
    // Course found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221"};
    routeController.findCourseLocation(req, res);

    EXPECT_EQ(res.code, 200);
    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());

    std::string location = courseIt->second->getCourseLocation();
    std::string expectedBody = location + " is where the course is located.";
    EXPECT_EQ(res.body, expectedBody);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221"};
    routeController.findCourseLocation(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000"};
    routeController.findCourseLocation(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}

TEST_F(RouteControllerUnitTests, FindCourseInstructorTest) {
    // Course found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221"};
    routeController.findCourseInstructor(req, res);

    EXPECT_EQ(res.code, 200);
    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());
    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());

    std::string instructorName = courseIt->second->getInstructorName();
    std::string expectedBody = instructorName + " is the instructor for the course.";
    EXPECT_EQ(res.body, expectedBody);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221"};
    routeController.findCourseInstructor(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000"};
    routeController.findCourseInstructor(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
    res = crow::response();
}


TEST_F(RouteControllerUnitTests, FindCourseTimeTest) {
    // Course found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221"};
    routeController.findCourseTime(req, res);

    EXPECT_EQ(res.code, 200);
    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());
    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());

    std::string timeSlot = courseIt->second->getCourseTimeSlot();
    std::string expectedBody = "The course meets at: " + timeSlot;
    EXPECT_EQ(res.body, expectedBody);
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221"};
    routeController.findCourseTime(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000"};
    routeController.findCourseTime(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
    res = crow::response();
}

TEST_F(RouteControllerUnitTests, SetEnrollmentCountTest) {
    // Course not full
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221&count=100"};
    routeController.setEnrollmentCount(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully.");

    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());
    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());

    EXPECT_FALSE(courseIt->second->isCourseFull());
    res = crow::response();  

    // Course full
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221&count=150"};
    routeController.setEnrollmentCount(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully.");

    departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    coursesMapping = deptIt->second.getCourseSelection();
    courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());

    EXPECT_TRUE(courseIt->second->isCourseFull());
    res = crow::response();  

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221&count=150"};
    routeController.setEnrollmentCount(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();  

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000&count=150"};
    routeController.setEnrollmentCount(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}


TEST_F(RouteControllerUnitTests, SetCourseLocationTest) {
    // Locationally set successfully
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221&location=NewLocation"};
    routeController.setCourseLocation(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully.");

    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");
    ASSERT_NE(courseIt, coursesMapping.end());
    EXPECT_EQ(courseIt->second->getCourseLocation(), "NewLocation");
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221&location=NewLocation"};
    routeController.setCourseLocation(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000&location=NewLocation"};
    routeController.setCourseLocation(req, res);
    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}


TEST_F(RouteControllerUnitTests, SetCourseInstructorTest) {
    // Course found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221&instructor=NewInstructor"};
    routeController.setCourseInstructor(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully.");

    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");    
    EXPECT_EQ(courseIt->second->getInstructorName(), "NewInstructor");
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221&instructor=NewInstructor"};
    routeController.setCourseInstructor(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000&instructor=NewInstructor"};
    routeController.setCourseInstructor(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}


TEST_F(RouteControllerUnitTests, SetCourseTimeTest) {
    // Course found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221&time=10:00-11:15"};
    routeController.setCourseTime(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully.");

    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");    
    EXPECT_EQ(courseIt->second->getCourseTimeSlot(), "10:00-11:15");
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221&time=10:00-11:15"};
    routeController.setCourseTime(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
    res = crow::response();

    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000&time=10:00-11:15"};
    routeController.setCourseTime(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
}

TEST_F(RouteControllerUnitTests, RemoveMajorFromDeptTest) {
    // Department found
    req.url_params = crow::query_string{"?deptCode=PHYS"};
    routeController.removeMajorFromDept(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully");
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND"};
    routeController.removeMajorFromDept(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
}

TEST_F(RouteControllerUnitTests, AddMajorToDeptTest) {
    // Department found
    req.url_params = crow::query_string{"?deptCode=PHYS"};
    routeController.addMajorToDept(req, res);

    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Attribute was updated successfully");
    res = crow::response();

    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND"};
    routeController.addMajorToDept(req, res);

    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
}

TEST_F(RouteControllerUnitTests, DropStudentFromCourseTest) {
    // Drop from course with enrolled students
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=1221"};
    
    auto departmentMapping = MyApp::getDatabase()->getDepartmentMapping();
    auto deptIt = departmentMapping.find("PHYS");
    ASSERT_NE(deptIt, departmentMapping.end());

    auto coursesMapping = deptIt->second.getCourseSelection();
    auto courseIt = coursesMapping.find("1221");    
    EXPECT_EQ(courseIt->second->getCourseTimeSlot(), "10:00-11:15");
    courseIt->second->setEnrolledStudentCount(10); // Set enrolled students to 10
    routeController.dropStudentFromCourse(req, res);
    
    EXPECT_EQ(res.code, 200);
    EXPECT_EQ(res.body, "Student has been dropped");
    res = crow::response();
    
    // Drop a student from a course with zero enrolled students
    courseIt->second->setEnrolledStudentCount(0);
    
    routeController.dropStudentFromCourse(req, res);
    
    EXPECT_EQ(res.code, 400);
    EXPECT_EQ(res.body, "Student has not been dropped");
    res = crow::response();
    
    // Course not found
    req.url_params = crow::query_string{"?deptCode=PHYS&courseCode=0000"};
    routeController.dropStudentFromCourse(req, res);
    
    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Course Not Found");
    res = crow::response();
    
    // Department not found
    req.url_params = crow::query_string{"?deptCode=NOTFOUND&courseCode=1221"};
    routeController.dropStudentFromCourse(req, res);
    
    EXPECT_EQ(res.code, 404);
    EXPECT_EQ(res.body, "Department Not Found");
}
