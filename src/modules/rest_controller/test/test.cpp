#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <waffle/modules/rest_controller/rest_controller.h>
#include "employee/employee.h"

namespace {

class MockEmployeeRepository : public model::IEmployeeRepository {
public:
    MOCK_METHOD(void, Add, (model::Employee&&), (override));
    MOCK_METHOD(std::optional<model::Employee>, FindById, (size_t), (override));
    MOCK_METHOD(std::vector<model::Employee>, FindAll, (), (override));
    MOCK_METHOD(void, DeleteById, (size_t), (override));
};

} // namespace

TEST(RestController, Employee) {
    auto repository = std::make_shared<MockEmployeeRepository>();
    auto employeeController = std::make_shared<model::EmployeeController>(repository);

    auto req = Waffle::HttpRequest{
        .Method = "GET",
        .Path = "/employees",
    };

    std::cerr << "before" << std::endl;
    ProcessRequest(*employeeController, req);
    std::cerr << "after" << std::endl;
}
