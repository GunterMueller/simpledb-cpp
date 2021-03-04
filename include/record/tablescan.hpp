#pragma once

#include <memory>
#include <string>

#include "tx/transaction.hpp"
#include "record/layout.hpp"
#include "record/recordpage.hpp"
#include "record/rid.hpp"
#include "query/updatescan.hpp"

namespace smartdb {
  class table_scan: public update_scan {
  public:
    table_scan(std::shared_ptr<transaction> pTx, const std::string &pTableName, std::shared_ptr<layout> pLayout);
    ~table_scan();
    void before_first() override;
    bool next() override;
    int get_int(const std::string &pFldName) override;
    std::string get_string(const std::string &pFldName) override;
    constant get_val(const std::string &pFldName) override;
    bool has_field(const std::string &pFldName) override;
    void close() override;

    void set_int(const std::string &pFldName, const int &pVal) override;
    void set_string(const std::string &pFldName, const std::string &pVal) override;
    void set_val(const std::string &pFldName, const constant &pVal) override;
    void insert() override;
    void remove() override;
    rid get_rid() override;
    void move_to_rid(const rid &pRID) override;

  private:
    std::shared_ptr<transaction> mTx;
    std::shared_ptr<layout> mLt;
    std::shared_ptr<record_page> mRP;
    std::string mFileName;
    int mCurrentSlot;
    
    void move_to_block(const int &pBlkNum);
    void move_to_new_block();
    bool at_last_block();
  };
}
