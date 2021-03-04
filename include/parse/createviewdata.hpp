#pragma once

#include <string>
#include <memory>

#include "parse/querydata.hpp"
#include "parse/object.hpp"

namespace smartdb {
  class create_view_data: public object {
  public:
    int op() override;
    // create_view_data();
    // create_view_data(const create_view_data &pCVD);
    create_view_data(const std::string &pViewName, std::shared_ptr<query_data> pQD);
    // create_view_data& operator=(const create_view_data &pCVD);
    std::string view_name() const;
    std::string view_def() const;

  private:
    std::string mViewName;
    std::shared_ptr<query_data> mQD;
  };
}
