#ifndef THIRD_PARTY_CEL_CPP_EVAL_PUBLIC_UNKNOWN_FUNCTION_RESULT_SET_H_
#define THIRD_PARTY_CEL_CPP_EVAL_PUBLIC_UNKNOWN_FUNCTION_RESULT_SET_H_

#include <vector>

#include "google/api/expr/v1alpha1/syntax.pb.h"
#include "eval/public/cel_function.h"

namespace google {
namespace api {
namespace expr {
namespace runtime {

// Represents a function result that is unknown at the time of execution. This
// allows for lazy evaluation of expensive functions.
class UnknownFunctionResult {
 public:
  UnknownFunctionResult(const CelFunctionDescriptor& descriptor, int64_t expr_id,
                        const std::vector<CelValue>& arguments)
      : descriptor_(descriptor), expr_id_(expr_id), arguments_(arguments) {}

  // The descriptor of the called function that return Unknown.
  const CelFunctionDescriptor& descriptor() const { return descriptor_; }

  // The id of the |Expr| that triggered the function call step. Provided
  // informationally -- if two different |Expr|s generate the same unknown call,
  // they will be treated as the same unknown function result.
  int64_t call_expr_id() const { return expr_id_; }

  // The arguments of the function call that generated the unknown.
  const std::vector<CelValue>& arguments() const { return arguments_; }

  // Equality operator provided for set semantics.
  // Compares descriptor then arguments elementwise.
  bool IsEqualTo(const UnknownFunctionResult& other) const;

 private:
  CelFunctionDescriptor descriptor_;
  int64_t expr_id_;
  std::vector<CelValue> arguments_;
};

// Represents a collection of unknown function results at a particular point in
// execution. Execution should advance further if this set of unknowns are
// provided. It may not advance if only a subset are provided.
// Set semantics use |IsEqualTo()| defined on |UnknownFunctionResult|.
class UnknownFunctionResultSet {
 public:
  // Empty set
  UnknownFunctionResultSet() {}

  // Merge constructor -- effectively union(lhs, rhs).
  UnknownFunctionResultSet(const UnknownFunctionResultSet& lhs,
                           const UnknownFunctionResultSet& rhs);

  // Initialize with a single UnknownFunctionResult.
  UnknownFunctionResultSet(const UnknownFunctionResult* initial)
      : unknown_function_results_{initial} {}

  const std::vector<const UnknownFunctionResult*>& unknown_function_results()
      const {
    return unknown_function_results_;
  }

 private:
  std::vector<const UnknownFunctionResult*> unknown_function_results_;
  void Add(const UnknownFunctionResult* result);
};

}  // namespace runtime
}  // namespace expr
}  // namespace api
}  // namespace google

#endif  // THIRD_PARTY_CEL_CPP_EVAL_PUBLIC_UNKNOWN_FUNCTION_RESULT_SET_H_
