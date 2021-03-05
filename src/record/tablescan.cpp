#include "record/tablescan.hpp"

namespace smartdb {
  table_scan::table_scan(std::shared_ptr<transaction> pTx, const std::string &pTableName, std::shared_ptr<layout> pLt):
    mTx(pTx), mLt(pLt)
  {
    mFileName = pTableName + ".tbl";
    if (mTx->size(mFileName) == 0) {
      move_to_new_block();
    } else {
      move_to_block(0);
    }
  }

  table_scan::~table_scan() {}

  void table_scan::before_first() {
    move_to_block(0);
  }

  bool table_scan::next() {
    mCurrentSlot = mRP->next_after(mCurrentSlot);
    while (mCurrentSlot < 0) {
      if (at_last_block()) {
        return false;
      }
      move_to_block(mRP->block()->number() + 1);
      mCurrentSlot = mRP->next_after(mCurrentSlot);
    }
    return true;
  }

  
  int table_scan::get_int(const std::string &pFldName) {
    return mRP->get_int(mCurrentSlot, pFldName);
  }

  std::string table_scan::get_string(const std::string &pFldName) {
    return mRP->get_string(mCurrentSlot, pFldName);
  }

  constant table_scan::get_val(const std::string &pFldName) {
    if (mLt->get_schema()->type(pFldName) == schema::integer) {
      return constant(get_int(pFldName));
    } else {
      return constant(get_string(pFldName));
    }
  }

  bool table_scan::has_field(const std::string &pFldName) {
    return mLt->get_schema()->has_field(pFldName);
  }

  void table_scan::close() {
    if (mRP) {
      mTx->unpin(*(mRP->block())); // todo fix
    }
  }

  void table_scan::set_int(const std::string &pFldName, const int &pVal) {
    return mRP->set_int(mCurrentSlot, pFldName, pVal);
  }

  void table_scan::set_string(const std::string &pFldName, const std::string &pVal) {
    return mRP->set_string(mCurrentSlot, pFldName, pVal);
  }

  void table_scan::set_val(const std::string &pFldName, const constant &pVal) {
    if (mLt->get_schema()->type(pFldName) == schema::integer) {
      set_int(pFldName, pVal.as_int());
    } else {
      set_string(pFldName, pVal.as_string());
    }
  }

  void table_scan::insert() {
    mCurrentSlot = mRP->insert_after(mCurrentSlot);
    while (mCurrentSlot < 0) {
      if (at_last_block()) {
        move_to_new_block();
      } else {
        move_to_block(mRP->block()->number()+1);
      }
      mCurrentSlot = mRP->insert_after(mCurrentSlot);
    }
  }

  void table_scan::remove() {
    mRP->remove(mCurrentSlot);
  }

  rid table_scan::get_rid() {
    return rid(mRP->block()->number(), mCurrentSlot);
  }

  void table_scan::move_to_rid(const rid &pRID) {
    close();
    std::shared_ptr<block_id> blockId(new block_id(mFileName, pRID.block_number()));
    mRP = std::shared_ptr<record_page>(new record_page(mTx, blockId, mLt));
    mCurrentSlot = pRID.slot();
  }


  void table_scan::move_to_block(const int &pBlkNum) {
    close();
    std::shared_ptr<block_id> blockId(new block_id(mFileName, pBlkNum));
    mRP = std::shared_ptr<record_page>(new record_page(mTx, blockId, mLt));
    mCurrentSlot = -1;
  }

  void table_scan::move_to_new_block() {
    close();
    std::shared_ptr<block_id> blockId = std::make_shared<block_id>(mTx->append(mFileName)); // todo fix
    mRP = std::shared_ptr<record_page>(new record_page(mTx, blockId, mLt));
    mRP->format();
    mCurrentSlot = -1;
  }

  bool table_scan::at_last_block() {
    return mRP->block()->number() == (mTx->size(mFileName)-1);
  }

}
