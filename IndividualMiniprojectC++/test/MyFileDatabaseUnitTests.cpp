// Copyright (c) 2024 Annie Xu @ Columbia University

#include <gtest/gtest.h>
#include "MyFileDatabase.h"

class MyFileDatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto phys1221 = std::make_shared<Course>(150, "James G. Mccann", "301 PUP", "4:10-5:25");
        phys1221->setEnrolledStudentCount(118);

        auto phys2000 = std::make_shared<Course>(100, "Frank E. L. Banta", "402 CHANDLER", "1:10-3:40");
        phys2000->setEnrolledStudentCount(98);

        auto phys3801 = std::make_shared<Course>(150, "Katherine M. McMahon", "603 MUDD", "4:10-5:25");
        phys3801->setEnrolledStudentCount(96);

        std::map<std::string, std::shared_ptr<Course>> courses;
        courses["1221"] = phys1221;
        courses["2000"] = phys2000;
        courses["3801"] = phys3801;

        Department phys("PHYS", courses, "Marcia L. Newson", 200);
        mapping["PHYS"] = phys;

        database = new MyFileDatabase(1, ""); 
        database->setMapping(mapping);
    }

    void TearDown() override {
        delete database; 
    }

    MyFileDatabase* database;
    std::map<std::string, Department> mapping;
};

TEST_F(MyFileDatabaseTest, GetDepartmentMapping) {
    std::map<std::string, Department> receivedMapping = database->getDepartmentMapping();

    ASSERT_EQ(mapping.size(), receivedMapping.size());

    for (const auto& pair : mapping) {
        auto it = receivedMapping.find(pair.first);
        ASSERT_NE(it, receivedMapping.end());
        EXPECT_EQ(pair.second.display(), it->second.display());
    }
}
