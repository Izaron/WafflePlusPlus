#include <memory>
#include <optional>
#include <vector>

namespace model {

// @jsonable
struct Employee {
    size_t Id;
    std::string Name;
    double Salary;
};

class IEmployeeRepository {
public:
    virtual void Add(Employee employee) = 0;
    virtual std::optional<Employee> FindById(size_t id) = 0;
    virtual std::vector<Employee> FindAll() = 0;
    virtual void DeleteById(size_t id) = 0;
};

// @restcontroller
class EmployeeController {
public:
    EmployeeController(std::shared_ptr<IEmployeeRepository> repository)
        : repository_{std::move(repository)}
    {}

    /*
     * @brief Add a new employee
     * @postmapping /employees
     */
    void Add(/* @requestbody */ Employee employee) {
        repository_->Add(std::move(employee));
    }

    /*
     * @brief Get the employee with given ID
     * @getmapping /employee/{id}
     */
    std::optional<Employee> FindById(/* @pathvariable */ size_t id) {
        return repository_->FindById(id);
    }

    /*
     * @brief Get all employees
     * @getmapping /employees
     */
    std::vector<Employee> FindAll() {
        return repository_->FindAll();
    }

    /*
     * @brief Delete the employee with given ID
     * @deletemapping /employee/{id}
     */
    void DeleteById(/* @pathvariable */ size_t id) {
        repository_->DeleteById(id);
    }

private:
    std::shared_ptr<IEmployeeRepository> repository_;
};

} // namespace model
