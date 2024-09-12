// Copyright (c) 2024 Annie Xu @ Columbia University

#include <gtest/gtest.h>
#include "Department.h"

class DepartmentUnitTests : public ::testing::Test {
protected:
    static Department* phys;

public:
    static std::map<std::string, std::shared_ptr<Course>> courses;

    static void SetUpTestSuite() {
        auto phys1221 = std::make_shared<Course>(150, "James G. Mccann", "301 PUP", "4:10-5:25");
        phys1221->setEnrolledStudentCount(118);

        auto phys2000 = std::make_shared<Course>(100, "Frank E. L. Banta", "402 CHANDLER", "1:10-3:40");
        phys2000->setEnrolledStudentCount(98);

        auto phys3801 = std::make_shared<Course>(150, "Katherine M. McMahon", "603 MUDD", "4:10-5:25");
        phys3801->setEnrolledStudentCount(96);

        courses["1221"] = phys1221;
        courses["2000"] = phys2000;
        courses["3801"] = phys3801;

        phys = new Department("PHYS", courses, "Marcia L. Newson", 200);
    }

    static void TearDownTestSuite() {
        delete phys;
        phys = nullptr;
    }
};
    
Department* DepartmentUnitTests::phys = nullptr;
std::map<std::string, std::shared_ptr<Course>> DepartmentUnitTests::courses;

TEST_F(DepartmentUnitTests, DisplayTest) {
    std::string expectedResult =
        "PHYS 1221: \nInstructor: James G. Mccann; Location: 301 PUP; Time: 4:10-5:25\n"
        "PHYS 2000: \nInstructor: Frank E. L. Banta; Location: 402 CHANDLER; Time: 1:10-3:40\n"
        "PHYS 3801: \nInstructor: Katherine M. McMahon; Location: 603 MUDD; Time: 4:10-5:25\n";
    ASSERT_EQ(expectedResult, phys->display());
}

TEST_F(DepartmentUnitTests, ConstructorTest) {
    Department dept;

    EXPECT_EQ(0, dept.getNumberOfMajors()); 
    EXPECT_EQ("", dept.getDepartmentChair()); 
}

TEST_F(DepartmentUnitTests, GetCourseSelectionTest) {
    auto phys1221 = std::make_shared<Course>(150, "James G. Mccann", "301 PUP", "4:10-5:25");
    phys1221->setEnrolledStudentCount(118);

    auto phys2000 = std::make_shared<Course>(100, "Frank E. L. Banta", "402 CHANDLER", "1:10-3:40");
    phys2000->setEnrolledStudentCount(98);

    auto phys3801 = std::make_shared<Course>(150, "Katherine M. McMahon", "603 MUDD", "4:10-5:25");
    phys3801->setEnrolledStudentCount(96);

    std::map<std::string, std::shared_ptr<Course>> expectedCourses;
    expectedCourses["1221"] = phys1221;
    expectedCourses["2000"] = phys2000;
    expectedCourses["3801"] = phys3801;

    auto retrievedCourses = phys->getCourseSelection();

    EXPECT_EQ(courses,retrievedCourses);
}

TEST_F(DepartmentUnitTests, GetDepartmentChairTest) {
    ASSERT_EQ("Marcia L. Newson", phys->getDepartmentChair());
}

TEST_F(DepartmentUnitTests, GetNumberOfMajorsTest) {
    ASSERT_EQ(200, phys->getNumberOfMajors());
}

TEST_F(DepartmentUnitTests, CreateCourseTest) {
    phys->createCourse("1001", "Szabolcs Marka", "301 PUP", "2:40-3:55", 150);
    
    std::string expectedResult =
        "PHYS 1001: \nInstructor: Szabolcs Marka; Location: 301 PUP; Time: 2:40-3:55\n"
        "PHYS 1221: \nInstructor: James G. Mccann; Location: 301 PUP; Time: 4:10-5:25\n"
        "PHYS 2000: \nInstructor: Frank E. L. Banta; Location: 402 CHANDLER; Time: 1:10-3:40\n"
        "PHYS 3801: \nInstructor: Katherine M. McMahon; Location: 603 MUDD; Time: 4:10-5:25\n";

    ASSERT_EQ(expectedResult, phys->display());
}

TEST_F(DepartmentUnitTests, DropPersonFromMajorTest) {
    for (int i = 0; i < 200; ++i) {
        phys->dropPersonFromMajor();
    }
    ASSERT_EQ(0, phys->getNumberOfMajors());
    phys->dropPersonFromMajor();
    ASSERT_EQ(0, phys->getNumberOfMajors());
}

TEST_F(DepartmentUnitTests, AddPersonFromMajorTest) {
    for (int i = 0; i < 200; ++i) {
        phys->addPersonToMajor();
    }
    ASSERT_EQ(200, phys->getNumberOfMajors());
}

TEST_F(DepartmentUnitTests, SerializationDeserializeTest) {
    // serialize object to string
    std::ostringstream outStream;
    phys->serialize(outStream);
    std::string serializedData = outStream.str();

    // deserialize string
    std::istringstream inStream(serializedData);
    Department deserializedDept("", {}, "", 0);
    deserializedDept.deserialize(inStream);

    // verify deserialized string
    EXPECT_EQ(phys->getDepartmentChair(), deserializedDept.getDepartmentChair());
    EXPECT_EQ(phys->getNumberOfMajors(), deserializedDept.getNumberOfMajors());

    auto originalCourses = phys->getCourseSelection();
    auto deserializedCourses = deserializedDept.getCourseSelection();
    
    EXPECT_EQ(originalCourses.size(), deserializedCourses.size());

    for (const auto& entry : originalCourses) {
        const std::string& key = entry.first;
        const std::shared_ptr<Course>& originalCourse = entry.second;

        auto it = deserializedCourses.find(key);
        EXPECT_NE(it, deserializedCourses.end());
        if (it != deserializedCourses.end()) {
            const std::shared_ptr<Course>& deserializedCourse = it->second;
            EXPECT_EQ(originalCourse->getInstructorName(), deserializedCourse->getInstructorName());
            EXPECT_EQ(originalCourse->getCourseLocation(), deserializedCourse->getCourseLocation());
            EXPECT_EQ(originalCourse->getCourseTimeSlot(), deserializedCourse->getCourseTimeSlot());
            EXPECT_EQ(originalCourse->isCourseFull(), deserializedCourse->isCourseFull());
        }
    }
}
