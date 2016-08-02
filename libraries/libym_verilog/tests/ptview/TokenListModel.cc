
/// @file libym_verilog/tests/vlview/TokenListModel.cc
/// @brief TokenListModel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TokenListModel.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "TokenListModel.h"
#include "Lex.h"
#include "parser.h"
#include "RsrvWordDic.h"

  
// @brief コンストラクタ
// @param[in] parent 親のオブジェクト
TokenListModel::TokenListModel(QObject* parent) :
  QAbstractTableModel(parent)
{
}

// @brief デストラクタ
TokenListModel::~TokenListModel()
{
}

// @brief コラム数を返す．
int
TokenListModel::columnCount(const QModelIndex& /* parent */) const
{
  return 2;
}

// @brief 行数を返す．
int
TokenListModel::rowCount(const QModelIndex& /* parent */) const
{
  return mList.size();
}

// @brief 該当するデータを返す．
QVariant
TokenListModel::data(const QModelIndex& index,
		     int role) const
{
  if ( !index.isValid() ) {
    return QVariant();
  }

  int col = index.column();
  if ( role == Qt::TextAlignmentRole ) {
    if ( col == 0 ) {
      return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
    else {
      return int(Qt::AlignLeft | Qt::AlignVCenter);
    }
  }
  else if ( role == Qt::DisplayRole ) {
    using nsYm::nsVerilog::RsrvWordDic;
    const RsrvWordDic& dic = RsrvWordDic::the_dic();
  
    int row = index.row();
    const Token& token = mList.at(row);
  
    QString str;
    if ( col == 0 ) {
      switch ( token.mId ) {
      case SPACE:
	str = "SPACE";
	break;
	
      case NL:
	str = "NL";
	break;
	
      case COMMENT1:
	str = "COMMENT1";
	break;
	
      case COMMENT2:
	str = "COMMENT2";
	break;
	
      case IDENTIFIER:
	str = "IDENTIFIER";
	break;
	
      case SYS_IDENTIFIER:
	str = "SYS_IDENTIFIER";
	break;
	
      case UNUMBER:
	str = "UNUMBER";
	break;
	
      case UNUM_INT:
	str = "UNUM_INT";
	break;
	
      case RNUMBER:
	str = "RNUMBER";
	break;
	
      case STRING:
	str = "STRING";
	break;
	
      case CD_SYMBOL:
	str = "CD_SYMBOL";
	break;
	
      case ERROR:
	str = "ERROR";
	break;
	
      default:
	str = "RESERVED";
	break;
      }
    }
    else {
      switch ( token.mId ) {
      case COMMENT1:
      case COMMENT2:
      case IDENTIFIER:
      case SYS_IDENTIFIER:
      case UNUMBER:
      case UNUM_INT:
      case RNUMBER:
      case STRING:
      case CD_SYMBOL:
	str = token.mStr;
	break;
	
      default:
	if ( dic.str(token.mId) == NULL ) {
	  str = "???";
	}
	else {
	  str = dic.str(token.mId);
	}
	break;
      }
    }
    return str;
  }
  else {
    return QVariant();
  }
}
  
// @brief トークンを追加する．
// @param[in] id トークン番号
// @param[in] file_region トークンのファイル上の位置
// @param[in] str 文字列
void
TokenListModel::add_token(int id,
			  const FileRegion& file_region,
			  const char* str)
{
  int row = mList.size();
  mList.push_back(Token(id, file_region, str));
  QModelIndex index1 = index(row, 0);
  QModelIndex index2 = index(row, 1);
  emit dataChanged(index1, index2);
}
  
// @brief トークンのファイル上の位置を返す．
const TokenListModel::FileRegion&
TokenListModel::loc(int row_pos)
{
  return mList.at(row_pos).mLoc;
}

TokenListModel::Token::Token(int id,
			     const FileRegion& file_region,
			     const char* str) :
  mId(id),
  mLoc(file_region),
  mStr(str)
{
}
