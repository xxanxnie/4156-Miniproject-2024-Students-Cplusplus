// Copyright (c) 2024 Annie Xu @ Columbia University

#include <gtest/gtest.h>
#include "Course.h" 
#include <sstream>  // For std::ostringstream and std::istringstream

class CourseUnitTests : public ::testing::Test {
protected:
    static Course* coms1004;

    static void SetUpTestSuite() {
        coms1004 = new Course(400, "Adam Cannon", "417 IAB", "11:40-12:55");
    }

    static void TearDownTestSuite() {
        delete coms1004;
    }
};

Course* CourseUnitTests::coms1004 = nullptr;

TEST_F(CourseUnitTests, DisplayTest) {
    Course testCourse(250, "Griffin Newbold", "417 IAB", "11:40-12:55");
    std::string expectedResult = "\nInstructor: Griffin Newbold; Location: 417 IAB; Time: 11:40-12:55";
    ASSERT_EQ(expectedResult, testCourse.display());
}

TEST_F(CourseUnitTests, ConstructorTest) {
    Course coms3134{};
    EXPECT_EQ(coms3134.getCourseLocation(), "");
    EXPECT_EQ(coms3134.getInstructorName(), "");
    EXPECT_EQ(coms3134.getCourseTimeSlot(), "");
}

TEST_F(CourseUnitTests, GetCourseLocationTest) {
    EXPECT_EQ(coms1004->getCourseLocation(), "417 IAB");
}

TEST_F(CourseUnitTests, GetInstructorNameTest) {
    EXPECT_EQ(coms1004->getInstructorName(), "Adam Cannon");
}

TEST_F(CourseUnitTests, GetCourseTimeSlotTest) {
    EXPECT_EQ(coms1004->getCourseTimeSlot(), "11:40-12:55");
}

TEST_F(CourseUnitTests, ReassignInstructorTest) {
    coms1004->reassignInstructor("Brian Borowski");
    EXPECT_EQ(coms1004->getInstructorName(), "Brian Borowski");
}

TEST_F(CourseUnitTests, ReassignTimeTest) {
    coms1004->reassignTime("4:10-5:25");
    EXPECT_EQ(coms1004->getCourseTimeSlot(), "4:10-5:25");
}

TEST_F(CourseUnitTests, ReassignLocationTest) {
    coms1004->reassignLocation("301 URIS");
    EXPECT_EQ(coms1004->getCourseLocation(), "301 URIS");
}

TEST_F(CourseUnitTests, EnrolledStudentCountTest) {
    coms1004->setEnrolledStudentCount(399);
    EXPECT_FALSE(coms1004->isCourseFull());    // false since 399 < 400
    
    coms1004->enrollStudent();    // added enrolled student to 400, course should be full now
    EXPECT_TRUE(coms1004->isCourseFull());
    EXPECT_FALSE(coms1004->enrollStudent());  // shoudn't be able to add 
}


TEST_F(CourseUnitTests, DropStudentTest) {
    coms1004->setEnrolledStudentCount(400);
    EXPECT_TRUE(coms1004->isCourseFull());
    EXPECT_TRUE(coms1004->dropStudent());    // 400 to 399
    EXPECT_FALSE(coms1004->isCourseFull());   // originally full, after dropping a student, shouldn't be full anymore

    coms1004->setEnrolledStudentCount(0);    // no student enrolled, shouldn't be able to drop
    EXPECT_FALSE(coms1004->dropStudent());
}

TEST_F(CourseUnitTests, SerializationDeserializationTest) {
    coms1004->setEnrolledStudentCount(10);

    // serialize object to string
    std::ostringstream outStream;
    coms1004->serialize(outStream);
    std::string serializedData = outStream.str();

    // deserialize string
    std::istringstream inStream(serializedData);
    Course deserializedCourse{0, "", "", ""}; 
    deserializedCourse.deserialize(inStream);

    EXPECT_EQ(coms1004->getCourseLocation(), deserializedCourse.getCourseLocation());
    EXPECT_EQ(coms1004->getInstructorName(), deserializedCourse.getInstructorName());
    EXPECT_EQ(coms1004->getCourseTimeSlot(), deserializedCourse.getCourseTimeSlot());
    EXPECT_EQ(coms1004->isCourseFull(), deserializedCourse.isCourseFull());
}