#include <memory>
#include <optional>
#include <vector>

namespace model {

struct Employee {
    size_t Id;
    std::string Name;
    double Salary;
};

class IEmployeeRepository {
public:
    virtual void Add(Employee&& employee) = 0;
    virtual std::optional<Employee> FindById(size_t id) = 0;
    virtual std::vector<Employee> FindAll() = 0;
    virtual void DeleteById(size_t id) = 0;
};

// @restcontroller
class EmployeeController {
public:
    EmployeeController(std::shared_ptr<IEmployeeRepository> repository)
        : repository_{std::move(repository)}
    {
    }

    // @postmapping /employees
    void Add(Employee&& employee) {
        repository_->Add(std::move(employee));
    }

    // @getmapping /employees/{id}
    std::optional<Employee> FindById(size_t id) {
        return repository_->FindById(id);
    }

    // @getmapping /employees
    std::vector<Employee> FindAll() {
        return repository_->FindAll();
    }

    // @deletemapping /employees/{id}
    void DeleteById(size_t id) {
        repository_->DeleteById(id);
    }

private:
    std::shared_ptr<IEmployeeRepository> repository_;
};

} // namespace model
