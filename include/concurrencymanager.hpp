#pragma once

#include <memory>

#include "blockid.hpp"
#include "locktable.hpp"

namespace smartdb {
  class concurrency_manager{
  public:
    void slock(const block_id &pBlockId);
    void xlock(const block_id &pBlockId);
    void release();
  private:
    static lock_table mLockTable;
    std::map<block_id, std::string> mLocks;

    bool has_xlock(const block_id &pBlockId);
  };
}
