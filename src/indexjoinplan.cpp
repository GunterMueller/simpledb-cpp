#include "indexjoinplan.hpp"
#include "indexjoinscan.hpp"
#include "tablescan.hpp"

namespace smartdb {
  index_join_plan::index_join_plan(std::shared_ptr<plan> pP1, std::shared_ptr<plan> pP2, std::shared_ptr<index_info> pII, const std::string &pJoinField):
    mP1(pP1), mP2(pP2), mII(pII), mJoinField(pJoinField), mSch(std::shared_ptr<schema>(new schema))
  {
    mSch->add_all(mP1->get_schema());
    mSch->add_all(mP2->get_schema());
  }

  std::shared_ptr<scan> index_join_plan::open() {
    std::shared_ptr<scan> s = mP1->open();
    std::shared_ptr<table_scan> tS = std::dynamic_pointer_cast<table_scan>(mP2->open());
    if (!tS) {
      throw std::runtime_error("type conversion failed");
    }
    std::shared_ptr<index> idx = mII->open();
    return std::static_pointer_cast<scan>(std::shared_ptr<index_join_scan>(new index_join_scan(s, idx, mJoinField, tS)));
  }

  int index_join_plan::blocks_accessed() {
    return mP1->blocks_accessed() + mP1->records_output() * mII->blocks_accessed() + records_output();
  }

  int index_join_plan::records_output() {
    return mP1->records_output() * mII->records_output();
  }

  int index_join_plan::distinct_values(const std::string &pFldName) {
    if (mP1->get_schema()->has_field(pFldName)) {
      return mP1->distinct_values(pFldName);
    } else {
      return mP2->distinct_values(pFldName);
    }
  }

  std::shared_ptr<schema> index_join_plan::get_schema() {
    return mSch;
  }
}
