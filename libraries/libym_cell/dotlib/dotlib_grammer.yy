%{

/// @file libym_cell/dotlib/dotlib_grammer.yy
/// @brief .lib ファイル用パーザ
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: dotlib_grammer.yy 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/FileRegion.h"
#include "ym_utils/MsgHandler.h"

#include "DotLibParser.h"
#include "DotLibLex.h"


// より詳細なエラー情報を出力させる．
#define YYERROR_VERBOSE 1

// 位置を表す型
// (yylloc の型)
#define YYLTYPE FileRegion

// YYLTYPE を書き換えたので以下のマクロも書き換えが必要
#define YYLLOC_DEFAULT(Current, Rhs, N) Current = fr_merge(Rhs, N);


BEGIN_NAMESPACE_YM_CELL

// yacc/bison が生成したヘッダファイル
#include "dotlib_grammer.h"

// 字句解析関数
int
yylex(YYSTYPE*,
      YYLTYPE*,
      DotLibParser&);

// エラー報告関数
int
yyerror(YYLTYPE*,
	DotLibParser&,
	const char*);

// fr_array 全体のファイル領域を求める．
// 直感的には FileRegion(fr_array[1], fr_array[n])
// だが(先頭が 0 でなく 1 であることに注意),
// 場合によっては空のトークンで位置がないばあいがあるので
// それをスキップしなければならない．
inline
FileRegion
fr_merge(const FileRegion fr_array[],
	 size_t n)
{
  if ( n == 0 ) {
    // なんでこんなことがあるのか不明
    return FileRegion();
  }

  // 真の先頭を求める．
  size_t i;
  for (i = 1; i <= n && !fr_array[i].is_valid(); ++ i) ;
  const FileRegion& first = fr_array[i];

  // 真の末尾を求める．
  size_t j;
  for (j = n; i >= i && !fr_array[j].is_valid(); -- j) ;
  const FileRegion& last = fr_array[j];

  return FileRegion(first, last);
}

%}


//////////////////////////////////////////////////////////////////////
// bison の設定用のおまじない
//////////////////////////////////////////////////////////////////////

// "pure" parser にする．
%define api.pure

// 位置のトラッキングを行う．
%locations

// yyparse の引数
%parse-param {DotLibParser& parser}

// yylex の引数
%lex-param {DotLibParser& parser}


//////////////////////////////////////////////////////////////////////
// トークンの定義
//////////////////////////////////////////////////////////////////////

// トークンの型
%union {
  bool booltype;
  int inttype;
  double dbltype;
  const char* strtype;

  PtLibraryBody* libbody;
}

// 終端記号
%token INT_NUM
%token FLOAT_NUM
%token STR
%token DQ_STR
%token NL

// 予約語
%token AREA
%token AUXILIARY_PAD_CELL
%token BALANCED_TREE
%token BASE_TYPE
%token BEST_CASE_TREE
%token BIT_FROM
%token BIT_TO
%token BIT_WIDTH
%token BUNDLE
%token BUS
%token BUS_NAMING_STYLE
%token BUS_TYPE
%token CAPACITANCE
%token CAPACITIVE_LOAD_UNIT
%token CELL
%token CELL_DEGRADATION
%token CELL_FALL
%token CELL_FOOTPRINT
%token CELL_RISE
%token CLOCK
%token CLOCK_GATE_CLOCK_PIN
%token CLOCK_GATE_ENABLE_PIN
%token CLOCK_GATE_OBS_PIN
%token CLOCK_GATE_OUT_PIN
%token CLOCK_GATE_TEST_PIN
%token CMOS
%token CMOS2
%token COMMENT
%token COMPLEMENTARY_PIN
%token CONNECTION_CLASS
%token CONTENTION_CONDITION
%token CURRENT_UNIT
%token DATA_TYPE
%token DATE
%token DCM
%token DEFAULT
%token DEFINE_CELL_AREA
%token DELAY_MODEL
%token DIRECTION
%token DONT_FAULT
%token DONT_TOUCH
%token DONT_USE
%token DOWNTO
%token DRIVE_CURRENT
%token DRIVER_TYPE
%token FALSE
%token FALL_CAPACITANCE
%token FALL_CONSTRAINT
%token FALL_CURRENT_SLOPE_AFTER_THRESHOLD
%token FALL_CURRENT_SLOPE_BEFORE_THRESHOLD
%token FALL_DELAY_INTERCEPT
%token FALL_PIN_RESISTANCE
%token FALL_PROPAGATION
%token FALL_RESISTANCE
%token FALL_TIME_AFTER_THRESHOLD
%token FALL_TIME_BEFORE_THRESHOLD
%token FALL_TRANSITION
%token FANOUT_LENGTH
%token FANOUT_LOAD
%token FASTER_FACTOR
%token FAULT_MODEL
%token FPGA
%token FUNCTION
%token GENERIC_CMOS
%token HANDLE_NEGATIVE_CONSTRAINT
%token HYSTERESIS
%token IN_PLACE_SWAP_MODE
%token INDEX_1
%token INDEX_2
%token INDEX_3
%token INPUT_VOLTAGE
%token INPUT_THRESHOLD_PCT_FALL
%token INPUT_THRESHOLD_PCT_RISE
%token INTERCONNECT_DELAY
%token INTERMEDIATE_VALUES
%token INTERNAL_NODE
%token INTRINSIC_FALL
%token INTRINSIC_RISE
%token INVERTED_OUTPUT
%token IS_CLOCK_GATING_CELL
%token IS_PAD
%token LEAKAGE_POWER_UNIT
%token LIBRARY
%token LIBRARY_FEATURES
%token LU_TABLE_TEMPLATE
%token MAP_ONLY
%token MATCH_FOOTPRINT
%token MAX_CAPACITANCE
%token MAX_FANOUT
%token MAX_TRANSITION
%token MEMBERS
%token MIN_CAPACITANCE
%token MIN_FANOUT
%token MIN_PERIOD
%token MIN_PULSE_WIDTH_HIGH
%token MIN_PULSE_WIDTH_LOW
%token MIN_TRANSITION
%token MODE
%token MODE_DEFINITION
%token MODE_VALUE
%token MULTICELL_PAD_PIN
%token NO_SWAPPING
%token OPERATING_CONDITIONS
%token OUTPUT_VOLTAGE
%token OUTPUT_THRESHOLD_PCT_FALL
%token OUTPUT_THRESHOLD_PCT_RISE
%token PAD_CELL
%token PAD_TYPE
%token PIECE_DEFINE
%token PIECE_LENGTH
%token PIECE_PIN_CAP
%token PIECE_TOTAL_CAP
%token PIECE_TYPE
%token PIECE_WIRE_CAP
%token PIECEWISE_CMOS
%token PIN
%token PIN_EQUAL
%token PIN_FUNC_TYPE
%token PIN_OPPOSITE
%token POWER_LUT_TEMPLATE
%token PREFER_TIED
%token PREFERRED
%token PREFERRED_INPUT_PAD_VOLTAGE
%token PREFERRED_OUTPUT_PAD_SLEW_RATE_CONTROL
%token PREFERRED_OUTPUT_PAD_VOLTAGE
%token PROCESS
%token PULLING_CURRENT
%token PULLING_RESISTANCE
%token PULLING_RESISTANCE_UNIT
%token RELATED_BUS_PIN
%token RELATED_PIN
%token RESISTANCE
%token REVISION
%token RISE_CAPACITANCE
%token RISE_CONSTRAINT
%token RISE_CURRENT_SLOPE_AFTER_THRESHOLD
%token RISE_CURRENT_SLOPE_BEFORE_THRESHOLD
%token RISE_DELAY_INTERCEPT
%token RISE_PIN_RESISTANCE
%token RISE_PROPAGATION
%token RISE_RESISTANCE
%token RISE_TIME_AFTER_THRESHOLD
%token RISE_TIME_BEFORE_THRESHOLD
%token RISE_TRANSITION
%token ROUTING_LAYERS
%token ROUTING_TRACK
%token SCALED_CELL
%token SCALING_FACTORS
%token SCAN_GROUP
%token SDF_COND
%token SIMULATION
%token SINGLE_BIT_DEGENERATE
%token SLEW_CONTROL
%token SLEW_DERATE_FROM_LIBRARY
%token SLEW_LOWER_THRESHOLD_PCT_FALL
%token SLEW_LOWER_THRESHOLD_PCT_RISE
%token SLEW_UPPER_THRESHOLD_PCT_FALL
%token SLEW_UPPER_THRESHOLD_PCT_RISE
%token SLOPE
%token SLOPE_FALL
%token SLOPE_RISE
%token SLOWER_FACTOR
%token STATE_FUNCTION
%token STEADY_STATE_RESISTANCE_FLOAT_MAX
%token STEADY_STATE_RESISTANCE_FLOAT_MIN
%token STEADY_STATE_RESISTANCE_HIGH_MAX
%token STEADY_STATE_RESISTANCE_HIGH_MIN
%token STEADY_STATE_RESISTANCE_LOW_MAX
%token STEADY_STATE_RESISTANCE_LOW_MIN
%token TABLE_LOOKUP
%token TECHNOLOGY
%token TEMPERATURE
%token TEST_OUTPUT_ONLY
%token THREE_STATE
%token TIME_UNIT
%token TIMING
%token TIMING_RANGE
%token TIMING_SENSE
%token TIMING_TYPE
%token TOTAL_TRACK_AREA
%token TRACKS
%token TREE_TYPE
%token TRUE
%token TYPE
%token USE_FOR_SIZE_ONLY
%token VALUES
%token VARIABLE_1
%token VARIABLE_2
%token VARIABLE_3
%token VHDL_NAME
%token VOH
%token VOL
%token VOLTAGE
%token VOLTAGE_UNIT
%token VOMAX
%token VOMIN
%token WHEN
%token WIRE_LOAD
%token WIRE_LOAD_SELECTION
%token WORST_CASE_TREE
%token X_FUNCTION

// 字句解析器が返すエラーのトークン
%token ERROR

// 非終端ノードの型定義
%type <booltype> boolean_value
%type <inttype> technology
%type <inttype> delay_model


// 開始記号
%start file

%%

// ファイルは1つの library グループから構成される．
file
: library_group
;

// library グループ
library_group
: LIBRARY '(' STR ')' '{'
    library_head
    library_body
  '}'
{
  parser.set_libname(@$, $3);
}
;

// library 記述の先頭
library_head
: delay_model_attr
| technology_attr delay_model_attr
;

technology_attr
: TECHNOLOGY '(' technology ')' ';'
{
  //  parser.set_technology(@$, $3);
}
;

delay_model_attr
: DELAY_MODEL ':' delay_model ';'
{
  //parser.set_delay_model(@$, $3);
}

// library 記述の本体
library_body
: library_attr
| library_body library_attr
;

// library グループ・レベル属性
library_attr
: BUS_NAMING_STYLE ':' STR ';'
{
  //parser.set_bus_naming_style(@$, $3);
}
| ROUTING_LAYERS '(' STR ')' ';'
{
  //parser.set_routing_layers(@$, $3);
}
| DATE ':' STR ';'
{
  //parser.set_date(@$, $3);
}
| REVISION ':' FLOAT_NUM ';'
{
  //parser.set_revision(@$, $3);
}
| REVISION ':' STR ';'
{
  //parser.set_revision(@$, $3);
}
| COMMENT ':' STR ';'
{
  //parser.set_comment(@$, $3);
}
| TIME_UNIT ':' STR ';'
{
  //parser.set_time_unit(@$, $3);
}
| VOLTAGE_UNIT ':' STR ';'
{
  //parser.set_voltage_unit(@$, $3);
}
| CURRENT_UNIT ':' STR ';'
{
  //parser.set_current_unit(@$, $3);
}
| PULLING_RESISTANCE_UNIT ':' STR ';'
{
  //parser.set_pulling_resistance_unit(@$, $3);
}
| CAPACITIVE_LOAD_UNIT '(' INT_NUM ',' STR ')' ';'
{
  //parser.set_capacitive_load_unit(@$, $3, $5);
}
| LEAKAGE_POWER_UNIT ':' STR ';'
{
  //parser.set_leakage_power_unit(@$, $3);
}
| PIECE_TYPE ':' piece_type ';'
{
  //parser.set_piece_type(@$, $3);
}
| PIECE_DEFINE '(' STR ')' ';'
{
  //parser.set_piece_define(@$, $3);
}
| DEFINE_CELL_AREA '(' STR ',' resource_type ')' ';'
{
  //parser.set_define_cell_area(@$, $3, $5);
}
| IN_PLACE_SWAP_MODE ':' swap_mode ';'
{
  //parser.set_in_place_swap_mode(@$, $3);
}
| LIBRARY_FEATURES '(' STR ')' ';'
{
  //parser.set_library_features(@$, $3);
}
| SIMULATION ':' boolean_value ';'
{
  //parser.set_simulation(@$, $3);
}
| DEFAULT values
{
  //parser.set_default(@$, $2);
}
| OPERATING_CONDITIONS '(' STR ')' '{' operating_condition_description '}'
{
  //parser.set_operationg_conditions(@$, $3, $6);
}
| TIMING_RANGE '(' STR ')' '{' timing_range_description '}'
{
  //parser.set_timing_range(@$, $3, $6);
}
| WIRE_LOAD '(' STR ')' '{' wire_load_description '}'
{
  //parser.set_wire_load(@$, $3, $6);
}
| WIRE_LOAD_SELECTION '(' ')' '{' area_group_selections '}'
{
  //parser.set_wire_load_selection(@$, $5);
}
| LU_TABLE_TEMPLATE '(' STR ')' '{' lut_template_description '}'
{
  //parser.set_lu_table_template(@$, $3, $6);
}
| POWER_LUT_TEMPLATE '(' STR ')' '{' lut_template_description '}'
{
  //parser.set_power_lut_template(@$, $3, $6);
}
| CELL '(' STR ')' '{' cell_description '}'
{
  //parser.set_cell(@$, $3, $6);
}
| SCALED_CELL '(' STR ',' STR ')' '{' cell_description '}'
{
  //parser.set_scaled_cell(@$, $3, $5, $8);
}
| TYPE '(' STR ')' '{' bus_type_name '}'
{
  //parser.set_type(@$, $3, $6);
}
| INPUT_VOLTAGE '(' STR ')' '{' input_voltage_description '}'
{
  //parser.set_input_voltage(@$, $3, $6);
}
| OUTPUT_VOLTAGE '(' STR ')' '{' output_voltage_description '}'
{
  //parser.set_output_voltage(@$, $3, $6);
}
| INPUT_THRESHOLD_PCT_FALL ':' FLOAT_NUM ';'
{
  //parser.set_input_threshold_pct_fall(@$, $3);
}
| INPUT_THRESHOLD_PCT_RISE ':' FLOAT_NUM ';'
{
  //parser.set_input_threshold_pct_rise(@$, $3);
}
| OUTPUT_THRESHOLD_PCT_FALL ':' FLOAT_NUM ';'
{
  //parser.set_output_threshold_pct_fall(@$, $3);
}
| OUTPUT_THRESHOLD_PCT_RISE ':' FLOAT_NUM ';'
{
  //parser.set_output_threshold_pct_rise(@$, $3);
}
| SLEW_DERATE_FROM_LIBRARY ':' FLOAT_NUM ';'
{
  //parser.set_slew_defrate_from_library(@$, $3);
}
| SLEW_LOWER_THRESHOLD_PCT_FALL ':' FLOAT_NUM ';'
{
  //parser.set_slew_lower_threshold_pct_fall(@$, $3);
}
| SLEW_LOWER_THRESHOLD_PCT_RISE ':' FLOAT_NUM ';'
{
  //parser.set_slew_lower_threshold_pct_rise(@$, $3);
}
| SLEW_UPPER_THRESHOLD_PCT_FALL ':' FLOAT_NUM ';'
{
  //parser.set_slew_upper_threshold_pct_fall(@$, $3);
}
| SLEW_UPPER_THRESHOLD_PCT_RISE ':' FLOAT_NUM ';'
{
  //parser.set_slew_upper_threshold_pct_rise(@$, $3);
}
| PREFERRED_OUTPUT_PAD_SLEW_RATE_CONTROL ':' STR ';'
{
  //parser.set_preferred_output_pad_slew_rate_control(@$, $3);
}
| PREFERRED_OUTPUT_PAD_VOLTAGE ':' STR ';'
{
  //parser.set_preffered_output_pad_voltage(@$, $3);
}
| PREFERRED_INPUT_PAD_VOLTAGE ':' STR ';'
{
  //parser.set_preffered_input_pad_voltage(@$, $3);
}
;

// 嘘
resource_type
: STR
;

// テクノロジ
technology
: CMOS
{
  $$ = $1;
}
| FPGA
{
  $$ = $1;
}
;

// 遅延モデル
delay_model
: GENERIC_CMOS
{
  $$ = $1;
}
| TABLE_LOOKUP
{
  $$ = $1;
}
| CMOS2
{
  $$ = $1;
}
| PIECEWISE_CMOS
{
  $$ = $1;
}
| DCM
{
  $$ = $1;
}
;

// ブール値
boolean_value
: TRUE
{
  $$ = true;
}
| FALSE
{
  $$ = false;
}
;

// operating conditions
operating_condition_description
: PROCESS ':' FLOAT_NUM ';'
  TEMPERATURE ':' FLOAT_NUM ';'
  VOLTAGE ':' FLOAT_NUM ';'
  TREE_TYPE ':' tree_type
;

// tree type
tree_type
: WORST_CASE_TREE
| BEST_CASE_TREE
| BALANCED_TREE
;

// output voltage group
output_voltage_description
: VOL ':' FLOAT_NUM ';'
  VOH ':' FLOAT_NUM ';'
  VOMIN ':' FLOAT_NUM ';'
  VOMAX ':' FLOAT_NUM ';'
;

// timing_range_description
timing_range_description
: FASTER_FACTOR ':' FLOAT_NUM ';'
| SLOWER_FACTOR ':' FLOAT_NUM ';'
;

// wire_load_description
wire_load_description
: RESISTANCE ':' FLOAT_NUM ';'
  CAPACITANCE ':' FLOAT_NUM ';'
  AREA ':' FLOAT_NUM ';'
  SLOPE ':' FLOAT_NUM ';'
  FANOUT_LENGTH '(' INT_NUM ',' FLOAT_NUM ',' FLOAT_NUM ',' FLOAT_NUM ','
                    INT_NUM ')' ';'
  INTERCONNECT_DELAY '(' STR ')' '{'
    VALUES '(' float_values ')' ';'
  '}'
;

float_values
: FLOAT_NUM
| float_values ',' FLOAT_NUM
;

// 折れ線タイプ
piece_type
: PIECE_LENGTH
{
}
| PIECE_WIRE_CAP
{
}
| PIECE_PIN_CAP
{
}
| PIECE_TOTAL_CAP
{
}
;

// 消費電力計算用テンプレート
lut_template_description
: VARIABLE_1 ':' STR ';'
  INDEX_1 '(' STR ')' ';'
| VARIABLE_1 ':' STR ';' VARIABLE_2 ':' STR ';'
  INDEX_1 '(' STR ')' ';' INDEX_2 '(' STR ')' ';'
| VARIABLE_1 ':' STR ';' VARIABLE_2 ':' STR ';' VARIABLE_3 ':' STR ';'
  INDEX_1 '(' STR ')' ';' INDEX_2 '(' STR ')' ';' INDEX_3 '(' STR ')' ';'
;

// インプレースモード
swap_mode
: MATCH_FOOTPRINT
{
}
| NO_SWAPPING
{
}
;

// セルの情報
cell_description
: cell_attr
| cell_description cell_attr
;

// セルの属性
cell_attr
: AREA ':' FLOAT_NUM ';'
| CELL_FOOTPRINT ':' STR ';'
| CONTENTION_CONDITION ':' STR ';'
| DONT_FAULT ':' STR ';'
| DONT_TOUCH ':' boolean_value ';'
| DONT_USE ':' boolean_value ';'
| HANDLE_NEGATIVE_CONSTRAINT ':' boolean_value ';'
| IS_CLOCK_GATING_CELL ':' boolean_value ';'
| MAP_ONLY ':' boolean_value ';'
| PAD_CELL ':' boolean_value ';'
| AUXILIARY_PAD_CELL ':' boolean_value ';'
| PAD_TYPE ':' STR ';'
| PIN_EQUAL ':' '(' STR ')' ';'
| PIN_OPPOSITE ':' '(' STR ',' STR ')' ';'
| PREFERRED ':' boolean_value ';'
| SCALING_FACTORS ':' STR ';'
| SCAN_GROUP ':' STR ';'
| USE_FOR_SIZE_ONLY ':' boolean_value ';'
| VHDL_NAME ':' STR ';'
| MODE_DEFINITION '(' STR ')' '{' mode_values '}'
| ROUTING_TRACK '(' STR ')' '{'
    TRACKS ':' INT_NUM ';'
    TOTAL_TRACK_AREA ':' FLOAT_NUM ';'
  '}'
| PIN '(' pin_list ')' '{' pingroup_description '}'
| BUS '(' STR ')' '{' bus_description '}'
| BUNDLE '(' STR ')' '{' bundle_description '}'
| SINGLE_BIT_DEGENERATE ':' STR ';'
;

mode_values
: mode_value
| mode_values mode_value
;

mode_value
: MODE_VALUE '(' STR ')' '{'
    WHEN ':' STR ';'
    SDF_COND ':' STR ';'
  '}'
;

pin_list
: STR
| pin_list ',' STR
;

pingroup_description
: pingroup_attr
| pingroup_description pingroup_attr
;

pingroup_attr
: CAPACITANCE ':' FLOAT_NUM ';'
| FALL_CAPACITANCE ':' FLOAT_NUM ';'
| RISE_CAPACITANCE ':' FLOAT_NUM ';'
| CLOCK_GATE_CLOCK_PIN ':' boolean_value ';'
| CLOCK_GATE_ENABLE_PIN ':' boolean_value ';'
| CLOCK_GATE_OBS_PIN ':' boolean_value ';'
| CLOCK_GATE_OUT_PIN ':' boolean_value ';'
| CLOCK_GATE_TEST_PIN ':' boolean_value ';'
| COMPLEMENTARY_PIN ':' STR ';'
| CONNECTION_CLASS ':' STR ';'
| DIRECTION ':' STR ';'
| DONT_FAULT ':' STR ';'
| DRIVER_TYPE ':' STR ';'
| FAULT_MODEL ':' STR ';'
| INVERTED_OUTPUT ':' boolean_value ';'
| PIN_FUNC_TYPE ':' STR ';'
| STEADY_STATE_RESISTANCE_FLOAT_MAX ':' FLOAT_NUM ';'
| STEADY_STATE_RESISTANCE_FLOAT_MIN ':' FLOAT_NUM ';'
| STEADY_STATE_RESISTANCE_HIGH_MAX ':' FLOAT_NUM ';'
| STEADY_STATE_RESISTANCE_HIGH_MIN ':' FLOAT_NUM ';'
| STEADY_STATE_RESISTANCE_LOW_MAX ':' FLOAT_NUM ';'
| STEADY_STATE_RESISTANCE_LOW_MIN ':' FLOAT_NUM ';'
| TEST_OUTPUT_ONLY ':' boolean_value ';'
| FANOUT_LOAD ':' FLOAT_NUM ';'
| MAX_FANOUT ':' INT_NUM ';'
| MIN_FANOUT ':' INT_NUM ';'
| MAX_TRANSITION ':' FLOAT_NUM ';'
| MIN_TRANSITION ':' FLOAT_NUM ';'
| MAX_CAPACITANCE ':' FLOAT_NUM ';'
| MIN_CAPACITANCE ':' FLOAT_NUM ';'
| CELL_DEGRADATION '(' STR ')' '{'
    INDEX_1 '(' STR ')' ';'
    VALUES '(' STR ')' ';'
  '}'
| CLOCK ':' boolean_value ';'
| MIN_PERIOD ':' FLOAT_NUM ';'
| MIN_PULSE_WIDTH_HIGH ':' FLOAT_NUM ';'
| MIN_PULSE_WIDTH_LOW ':' FLOAT_NUM ';'
| FUNCTION ':' STR ';'
| THREE_STATE ':' STR ';'
| X_FUNCTION ':' STR ';'
| STATE_FUNCTION ':' STR ';'
| INTERNAL_NODE ':' STR ';'
| PREFER_TIED ':' STR ';'
| IS_PAD ':' boolean_value ';'
| MULTICELL_PAD_PIN ':' boolean_value ';'
| INPUT_VOLTAGE ':' STR ';'
| PULLING_RESISTANCE ':' FLOAT_NUM ';'
| PULLING_CURRENT ':' FLOAT_NUM ';'
| HYSTERESIS ':' boolean_value ';'
| DRIVE_CURRENT ':' FLOAT_NUM ';'
| SLEW_CONTROL ':' STR ';'
| RISE_CURRENT_SLOPE_BEFORE_THRESHOLD ':' FLOAT_NUM ';'
| RISE_CURRENT_SLOPE_AFTER_THRESHOLD ':' FLOAT_NUM ';'
| FALL_CURRENT_SLOPE_BEFORE_THRESHOLD ':' FLOAT_NUM ';'
| FALL_CURRENT_SLOPE_AFTER_THRESHOLD ':' FLOAT_NUM ';'
| RISE_TIME_BEFORE_THRESHOLD ':' FLOAT_NUM ';'
| RISE_TIME_AFTER_THRESHOLD ':' FLOAT_NUM ';'
| FALL_TIME_BEFORE_THRESHOLD ':' FLOAT_NUM ';'
| FALL_TIME_AFTER_THRESHOLD ':' FLOAT_NUM ';'
| TIMING '(' timing_label ')' '{' timing_description '}'
;

bus_type_name
: BASE_TYPE ':' STR ';' // STR は array のみ
| DATA_TYPE ':' STR ';' // STR は bit のみ
| BIT_WIDTH ':' INT_NUM ';'
| BIT_FROM ':' INT_NUM ';'
| BIT_TO ':' INT_NUM ';'
| DOWNTO ':' boolean_value ';'
;

bus_description
: bus_attr
| bus_description bus_attr
;

bus_attr
: BUS_TYPE ':' STR ';'
| pingroup_attr
| PIN '(' STR ')' '{' pingroup_description '}'
;

bundle_description
: bundle_attr
| bundle_description bundle_attr
;

bundle_attr
: MEMBERS '(' pin_list ')'
| pingroup_attr
;

timing_label
: /* 空 */
| timing_label ',' STR
;

timing_description
: timing_attr
| timing_description timing_attr
;

timing_attr
: RELATED_PIN ':' STR ';'
| RELATED_BUS_PIN ':' STR ';'
| TIMING_SENSE ':' STR ';'
| TIMING_TYPE ':' STR ';'
| MODE '(' STR ',' STR ')' ';'
| INTRINSIC_RISE ':' FLOAT_NUM ';'
| INTRINSIC_FALL ':' FLOAT_NUM ';'
| RISE_RESISTANCE ':' FLOAT_NUM ';'
| FALL_RESISTANCE ':' FLOAT_NUM ';'
| RISE_DELAY_INTERCEPT '(' INT_NUM ',' STR ')' ';'
| FALL_DELAY_INTERCEPT '(' INT_NUM ',' STR ')' ';'
| RISE_PIN_RESISTANCE '(' INT_NUM ',' STR ')' ';'
| FALL_PIN_RESISTANCE '(' INT_NUM ',' STR ')' ';'
| RISE_TRANSITION '(' STR ')' '{' values_attr '}'
| FALL_TRANSITION '(' STR ')' '{' values_attr '}'
| CELL_RISE '(' STR ')' '{' values_attr '}'
| CELL_FALL '(' STR ')' '{' values_attr '}'
| RISE_PROPAGATION '(' STR ')' '{' values_attr '}'
| FALL_PROPAGATION '(' STR ')' '{' values_attr '}'
| SLOPE_RISE ':' FLOAT_NUM ';'
| SLOPE_FALL ':' FLOAT_NUM ';'
| WHEN ':' STR ';'
| SDF_COND ':' STR ';'
| RISE_CONSTRAINT '(' STR ')' '{' values_attr '}'
| FALL_CONSTRAINT '(' STR ')' '{' values_attr '}'

values_attr
: VALUES '(' values ')' ';'
| INTERMEDIATE_VALUES '(' values ')' ';'
  VALUES '(' values ')' ';'
| VALUES '(' values ')' ';'
  INTERMEDIATE_VALUES '(' values ')' ';'
;

values
: STR
| values ',' STR
;

%%

// 字句解析器
// 実際には Lex が仕事をしている．
int
yylex(YYSTYPE* lvalp,
      YYLTYPE* llocp,
      DotLibParser& parser)
{
  return parser.yylex(*lvalp, *llocp);
}

// エラー出力関数
int
yyerror(YYLTYPE* llocp,
	DotLibParser& parser,
	const char* msg)
{
  string s2;
  // 好みの問題だけど "parse error" よりは "syntax error" の方が好き．
  if ( !strncmp(msg, "parse error", 11) ) {
    s2 ="syntax error";
    s2 += (msg + 11);
  }
  else {
    s2 = msg;
  }

  parser.put_msg(__FILE__, __LINE__,
		 *llocp,
		 kMsgError,
		 "PARS",
		 s2);

  return 1;
}

END_NAMESPACE_YM_CELL
