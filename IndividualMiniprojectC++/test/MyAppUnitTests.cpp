// Copyright (c) 2024 Annie Xu @ Columbia University

#include <gtest/gtest.h>
#include "MyApp.h"

const std::string expectedDatabase = R"(For the CHEM department:
CHEM 1403: 
Instructor: Ruben M Savizky; Location: 309 HAV; Time: 6:10-7:25
CHEM 1500: 
Instructor: Joseph C Ulichny; Location: 302 HAV; Time: 6:10-9:50
CHEM 2045: 
Instructor: Luis M Campos; Location: 209 HAV; Time: 1:10-2:25
CHEM 2444: 
Instructor: Christopher Eckdahl; Location: 309 HAV; Time: 11:40-12:55
CHEM 2494: 
Instructor: Talha Siddiqui; Location: 202 HAV; Time: 1:10-5:00
CHEM 3080: 
Instructor: Milan Delor; Location: 209 HAV; Time: 10:10-11:25
CHEM 4071: 
Instructor: Jonathan S Owen; Location: 320 HAV; Time: 8:40-9:55
CHEM 4102: 
Instructor: Dalibor Sames; Location: 320 HAV; Time: 10:10-11:25

For the COMS department:
COMS 1004: 
Instructor: Adam Cannon; Location: 417 IAB; Time: 11:40-12:55
COMS 3134: 
Instructor: Brian Borowski; Location: 301 URIS; Time: 4:10-5:25
COMS 3157: 
Instructor: Jae Lee; Location: 417 IAB; Time: 4:10-5:25
COMS 3203: 
Instructor: Ansaf Salleb-Aouissi; Location: 301 URIS; Time: 10:10-11:25
COMS 3251: 
Instructor: Tony Dear; Location: 402 CHANDLER; Time: 1:10-3:40
COMS 3261: 
Instructor: Josh Alman; Location: 417 IAB; Time: 2:40-3:55
COMS 3827: 
Instructor: Daniel Rubenstein; Location: 207 Math; Time: 10:10-11:25
COMS 4156: 
Instructor: Gail Kaiser; Location: 501 NWC; Time: 10:10-11:25

For the ECON department:
ECON 1105: 
Instructor: Waseem Noor; Location: 309 HAV; Time: 2:40-3:55
ECON 2257: 
Instructor: Tamrat Gashaw; Location: 428 PUP; Time: 10:10-11:25
ECON 3211: 
Instructor: Murat Yilmaz; Location: 310 FAY; Time: 4:10-5:25
ECON 3213: 
Instructor: Miles Leahey; Location: 702 HAM; Time: 4:10-5:25
ECON 3412: 
Instructor: Thomas Piskula; Location: 702 HAM; Time: 11:40-12:55
ECON 4415: 
Instructor: Evan D Sadler; Location: 309 HAV; Time: 10:10-11:25
ECON 4710: 
Instructor: Matthieu Gomez; Location: 517 HAM; Time: 8:40-9:55
ECON 4840: 
Instructor: Mark Dean; Location: 142 URIS; Time: 2:40-3:55

For the IEOR department:
IEOR 2500: 
Instructor: Uday Menon; Location: 627 MUDD; Time: 11:40-12:55
IEOR 3404: 
Instructor: Christopher J Dolan; Location: 303 MUDD; Time: 10:10-11:25
IEOR 3658: 
Instructor: Daniel Lacker; Location: 310 FAY; Time: 10:10-11:25
IEOR 4102: 
Instructor: Antonius B Dieker; Location: 209 HAM; Time: 10:10-11:25
IEOR 4106: 
Instructor: Kaizheng Wang; Location: 501 NWC; Time: 10:10-11:25
IEOR 4405: 
Instructor: Yuri Faenza; Location: 517 HAV; Time: 11:40-12:55
IEOR 4511: 
Instructor: Michael Robbins; Location: 633 MUDD; Time: 9:00-11:30
IEOR 4540: 
Instructor: Krzysztof M Choromanski; Location: 633 MUDD; Time: 7:10-9:40

For the PHYS department:
PHYS 1001: 
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

class MyAppTestFixture : public ::testing::Test {
protected:
    void TearDown() override {
        MyApp::onTermination(); 
    }
};

TEST_F(MyAppTestFixture, RunTest) {
    MyApp::run("setup");

    MyFileDatabase* database = MyApp::getDatabase();
    ASSERT_NE(database, nullptr);
    
    std::string actualDatabase = database->display();
    EXPECT_EQ(actualDatabase, expectedDatabase);
}

TEST_F(MyAppTestFixture, RunStartupTest) {
    MyApp::run("run");

    MyFileDatabase* database = MyApp::getDatabase();
    ASSERT_NE(database, nullptr);
    
    std::string actualDatabase = database->display();
    EXPECT_EQ(actualDatabase, expectedDatabase);
}

TEST_F(MyAppTestFixture, OverrideDatabase) {
    MyFileDatabase* newDatabase = new MyFileDatabase{1, ""};

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
    std::map<std::string, Department> mapping;
    mapping["PHYS"] = phys;

    newDatabase->setMapping(mapping);

    MyApp::overrideDatabase(newDatabase);

    MyFileDatabase* database = MyApp::getDatabase();
    EXPECT_EQ(database, newDatabase);
}

TEST_F(MyAppTestFixture, TerminationTest) {
    MyApp::run("run");

    MyFileDatabase* database = MyApp::getDatabase();
    ASSERT_NE(database, nullptr);

    MyApp::onTermination();

    MyFileDatabase* databaseTermination = MyApp::getDatabase();
    EXPECT_EQ(databaseTermination, nullptr);
}