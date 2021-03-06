/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#include "tensorflow/c/experimental/ops/math_ops.h"

#include "tensorflow/c/eager/abstract_context.h"
#include "tensorflow/c/eager/abstract_tensor_handle.h"
#include "tensorflow/c/eager/c_api_unified_experimental_internal.h"
#include "tensorflow/c/experimental/ops/array_ops.h"
#include "tensorflow/core/framework/types.h"
#include "tensorflow/core/platform/errors.h"
namespace tensorflow {
namespace ops {
using tensorflow::tracing::TracingOperation;

Status Mul(AbstractContext* ctx, absl::Span<AbstractTensorHandle* const> inputs,
           absl::Span<AbstractTensorHandle*> outputs, const char* name) {
  AbstractOperationPtr mul_op(ctx->CreateOperation());
  TF_RETURN_IF_ERROR(mul_op->Reset("Mul", /*raw_device_name=*/nullptr));
  if (isa<TracingOperation>(mul_op.get())) {
    TF_RETURN_IF_ERROR(
        dyn_cast<TracingOperation>(mul_op.get())->SetOpName(name));
  }
  TF_RETURN_IF_ERROR(mul_op->AddInput(inputs[0]));
  TF_RETURN_IF_ERROR(mul_op->AddInput(inputs[1]));
  int num_retvals = 1;
  return mul_op->Execute(outputs, &num_retvals);
}

Status Conj(AbstractContext* ctx,
            absl::Span<AbstractTensorHandle* const> inputs,
            absl::Span<AbstractTensorHandle*> outputs, const char* name) {
  auto dtype = inputs[0]->DataType();
  if (DataTypeIsFloating(BaseType(dtype)) ||
      DataTypeIsInteger(BaseType(dtype))) {
    TF_RETURN_IF_ERROR(Identity(ctx, inputs, outputs, name));
  } else {
    return errors::Unimplemented("Conj does not support complex types yet.");
  }
  return Status::OK();
}

}  // namespace ops
}  // namespace tensorflow
