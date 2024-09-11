// Copyright (c) 2024 Programming Systems Lab @ Columbia University

#include <gtest/gtest.h>
#include "Course.h" 
#include <sstream>  // For std::ostringstream and std::istringstream

class CourseUnitTests : public ::testing::Test {
protected:
    static Course* testCourse;

    static void SetUpTestSuite() {
        testCourse = new Course(250, "Griffin Newbold", "417 IAB", "11:40-12:55");
    }

    static void TearDownTestSuite() {
        delete testCourse;
    }
};

Course* CourseUnitTests::testCourse = nullptr;

TEST_F(CourseUnitTests, DisplayTest) {
    std::string expectedResult = "\nInstructor: Griffin Newbold; Location: 417 IAB; Time: 11:40-12:55";
    ASSERT_EQ(expectedResult, testCourse->display());
}

TEST_F(CourseUnitTests, ConstructorTest) {
    Course coms1004{};

    EXPECT_EQ(coms1004.getCourseLocation(), "");
    EXPECT_EQ(coms1004.getInstructorName(), "");
    EXPECT_EQ(coms1004.getCourseTimeSlot(), "");
}

TEST_F(CourseUnitTests, GetTest) {
    Course coms1004{400, "Adam Cannon", "417 IAB", "11:40-12:55"};

    EXPECT_EQ(coms1004.getCourseLocation(), "417 IAB");
    EXPECT_EQ(coms1004.getInstructorName(), "Adam Cannon");
    EXPECT_EQ(coms1004.getCourseTimeSlot(), "11:40-12:55");
}

TEST_F(CourseUnitTests, ReassignTest) {
    Course coms1004{400, "Adam Cannon", "417 IAB", "11:40-12:55"};

    coms1004.reassignInstructor("Brian Borowski");
    EXPECT_EQ(coms1004.getInstructorName(), "Brian Borowski");

    coms1004.reassignLocation("301 URIS");
    EXPECT_EQ(coms1004.getCourseLocation(), "301 URIS");

    coms1004.reassignTime("4:10-5:25");
    EXPECT_EQ(coms1004.getCourseTimeSlot(), "4:10-5:25");
}

TEST_F(CourseUnitTests, EnrollmentTest) {
    Course coms1004{400, "Adam Cannon", "417 IAB", "11:40-12:55"};

    coms1004.setEnrolledStudentCount(399);
    EXPECT_FALSE(coms1004.isCourseFull());    // false since 399 < 400
    
    coms1004.enrollStudent();    // added enrolled student to 400, course should be full now
    EXPECT_TRUE(coms1004.isCourseFull());
    EXPECT_FALSE(coms1004.enrollStudent());  // shoudn't be able to add 
}

TEST_F(CourseUnitTests, DropTest) {
    Course coms1004{400, "Adam Cannon", "417 IAB", "11:40-12:55"};
    
    coms1004.setEnrolledStudentCount(400);
    EXPECT_TRUE(coms1004.isCourseFull());
    EXPECT_TRUE(coms1004.dropStudent());    // 400 to 399
    EXPECT_FALSE(coms1004.isCourseFull());   // originally full, after dropping a student, shouldn't be full anymore

    coms1004.setEnrolledStudentCount(0);    // no student enrolled, shouldn't be able to drop
    EXPECT_FALSE(coms1004.dropStudent());
}

TEST_F(CourseUnitTests, SerializationDeserializationTest) {
    // Create a Course object with specific values
    Course coms1004{400, "Adam Cannon", "417 IAB", "11:40-12:55"};
    coms1004.setEnrolledStudentCount(10);

    // Serialize the object to a string stream
    std::ostringstream outStream;
    coms1004.serialize(outStream);
    std::string serializedData = outStream.str();

    // Deserialize the object from the string stream
    std::istringstream inStream(serializedData);
    Course deserializedCourse{0, "", "", ""}; // Initialize with default values
    deserializedCourse.deserialize(inStream);

    // Verify that the original and deserialized objects have the same state
    EXPECT_EQ(coms1004.getCourseLocation(), deserializedCourse.getCourseLocation());
    EXPECT_EQ(coms1004.getInstructorName(), deserializedCourse.getInstructorName());
    EXPECT_EQ(coms1004.getCourseTimeSlot(), deserializedCourse.getCourseTimeSlot());
    EXPECT_EQ(coms1004.isCourseFull(), deserializedCourse.isCourseFull());
}