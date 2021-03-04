#pragma once

#include <memory>

#include "tx/transaction.hpp"
#include "file/blockid.hpp"
#include "record/layout.hpp"
#include "record/rid.hpp"
#include "index/btree/direntry.hpp"
#include "index/btree/btpage.hpp"

namespace smartdb {
  class btree_leaf {
  public:
    btree_leaf(std::shared_ptr<transaction> pTx, std::shared_ptr<block_id> pBlk, std::shared_ptr<layout> pLt, const constant &pSeachKey);
    void close();
    bool next();
    rid get_data_rid();
    void remove(const rid &pDataRid);
    dir_entry insert(const rid &pDataRID);
    bool try_overflow();
  private:
    std::shared_ptr<transaction> mTx;
    std::shared_ptr<layout> mLt;
    constant mSearchKey;
    std::shared_ptr<bt_page> mContents;
    int mCurrentSlot;
    std::string mFileName;
  };
}
