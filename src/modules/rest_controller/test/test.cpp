#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <waffle/modules/rest_controller/rest_controller.h>
#include "employee/employee.h"

#include <iostream>

using ::testing::Invoke;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Throw;
using ::testing::_;

namespace {

class MockEmployeeRepository : public model::IEmployeeRepository {
public:
    MOCK_METHOD(void, Add, (model::Employee), (override));
    MOCK_METHOD(std::optional<model::Employee>, FindById, (size_t), (override));
    MOCK_METHOD(std::vector<model::Employee>, FindAll, (), (override));
    MOCK_METHOD(void, DeleteById, (size_t), (override));
};

class RestControllerEmployee : public ::testing::Test {
public:
    void SetUp() override {
        Repository = std::make_shared<StrictMock<MockEmployeeRepository>>();
        EmployeeController = std::make_shared<model::EmployeeController>(Repository);
    }

    std::shared_ptr<StrictMock<MockEmployeeRepository>> Repository;
    std::shared_ptr<model::EmployeeController> EmployeeController;
};

} // namespace

TEST_F(RestControllerEmployee, EmptyFindAll) {
    std::vector<model::Employee> emptyEmployees;
    EXPECT_CALL(*Repository, FindAll())
        .Times(1)
        .WillOnce(Return(emptyEmployees));

    auto req = Waffle::HttpRequest{
        .Method = "GET",
        .Path = "/employees",
    };

    const Waffle::HttpResponse response = ProcessRequest(*EmployeeController, req);
    ASSERT_EQ(response.StatusCode, 200);
    ASSERT_EQ(response.Body, R"([])");
}

TEST_F(RestControllerEmployee, SuccessFindAll) {
    std::vector<model::Employee> manyEmployees;
    manyEmployees.emplace_back(model::Employee{.Id = 13, .Name = "Mary", .Salary = 1234.56});
    manyEmployees.emplace_back(model::Employee{.Id = 26, .Name = "Ann", .Salary = 10000});
    manyEmployees.emplace_back(model::Employee{.Id = 39, .Name = "Jacqueline", .Salary = 7'777'777});
    EXPECT_CALL(*Repository, FindAll())
        .Times(1)
        .WillOnce(Return(manyEmployees));

    auto req = Waffle::HttpRequest{
        .Method = "GET",
        .Path = "/employees",
    };

    const Waffle::HttpResponse response = ProcessRequest(*EmployeeController, req);
    ASSERT_EQ(response.StatusCode, 200);
    ASSERT_EQ("\n" + response.Body, R"(
[
    {
        "id": 13,
        "name": "Mary",
        "salary": 1234.56
    },
    {
        "id": 26,
        "name": "Ann",
        "salary": 10000.0
    },
    {
        "id": 39,
        "name": "Jacqueline",
        "salary": 7777777.0
    }
])");
}

TEST_F(RestControllerEmployee, EmptyFindById) {
    EXPECT_CALL(*Repository, FindById(13071999))
        .Times(1)
        .WillOnce(Return(std::nullopt));

    auto req = Waffle::HttpRequest{
        .Method = "GET",
        .Path = "/employee/13071999",
    };

    const Waffle::HttpResponse response = ProcessRequest(*EmployeeController, req);
    ASSERT_EQ(response.StatusCode, 200);
    ASSERT_EQ(response.Body, R"(null)");
}

TEST_F(RestControllerEmployee, SuccessFindById) {
    model::Employee employer{.Id = 13071999, .Name = "Jim", .Salary = 7000};
    EXPECT_CALL(*Repository, FindById(13071999))
        .Times(1)
        .WillOnce(Return(employer));

    auto req = Waffle::HttpRequest{
        .Method = "GET",
        .Path = "/employee/13071999",
    };

    const Waffle::HttpResponse response = ProcessRequest(*EmployeeController, req);
    ASSERT_EQ(response.StatusCode, 200);
    ASSERT_EQ("\n" + response.Body, R"(
{
    "id": 13071999,
    "name": "Jim",
    "salary": 7000.0
})");
}

TEST_F(RestControllerEmployee, FailureDeleteById) {
    EXPECT_CALL(*Repository, DeleteById(13071999))
        .Times(1)
        .WillOnce(Throw(std::runtime_error("Can't delete missing user!")));

    auto req = Waffle::HttpRequest{
        .Method = "DELETE",
        .Path = "/employee/13071999",
    };

    const Waffle::HttpResponse response = ProcessRequest(*EmployeeController, req);
    ASSERT_EQ(response.StatusCode, 500);
    ASSERT_EQ("\n" + response.Body, R"(
{
    "reason": "Can't delete missing user!"
})");
}

TEST_F(RestControllerEmployee, Add) {
    std::optional<model::Employee> addedEmployee;
    const auto setEmployee = [&addedEmployee](model::Employee employee) {
        addedEmployee = std::move(employee);
    };
    EXPECT_CALL(*Repository, Add(_))
        .Times(1)
        .WillOnce(Invoke(setEmployee));

    auto req = Waffle::HttpRequest{
        .Method = "POST",
        .Path = "/employees",
        .Body = R"({"id": 13, "name": "John", "salary": 6000})",
    };

    const Waffle::HttpResponse response = ProcessRequest(*EmployeeController, req);
    ASSERT_EQ(response.StatusCode, 200);
    ASSERT_EQ(response.Body, R"()");

    ASSERT_TRUE(addedEmployee.has_value());
    ASSERT_EQ(addedEmployee->Id, 13);
    ASSERT_EQ(addedEmployee->Name, "John");
    ASSERT_EQ(addedEmployee->Salary, 6000);
}
