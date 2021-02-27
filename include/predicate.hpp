#pragma once

#include <memory>
#include <vector>

#include "term.hpp"
#include "scan.hpp"
#include "schema.hpp"
#include "plan.hpp"

namespace smartdb {
  class predicate {
  public:
    predicate();
    predicate(const predicate &pPred);
    predicate(const term &pT);
    predicate& operator=(const predicate &pPred);
    bool is_null() const;
    void con_join_with(const predicate &pP);
    bool is_satisfied(std::shared_ptr<scan> pS);
    int reduction_factor(std::shared_ptr<plan> pPlan);
    predicate select_sub_pred(std::shared_ptr<schema> pSch);
    predicate join_sub_pred(std::shared_ptr<schema> pSch1, std::shared_ptr<schema> pSch2);
    constant equates_with_constant(const std::string &pFldName);
    std::string equates_with_field(const std::string &pFldName);
    std::string to_string() const;
  private:
    std::vector<term> mTerms;
  };
}