# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: print.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='print.proto',
  package='luojianet_ms.prntpb',
  syntax='proto2',
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n\x0bprint.proto\x12\x13luojianet_ms.prntpb\"H\n\x0bTensorProto\x12\x0c\n\x04\x64ims\x18\x01 \x03(\x03\x12\x13\n\x0btensor_type\x18\x02 \x02(\t\x12\x16\n\x0etensor_content\x18\x03 \x02(\x0c\"\x8e\x01\n\x05Print\x12/\n\x05value\x18\x01 \x03(\x0b\x32 .luojianet_ms.prntpb.Print.Value\x1aT\n\x05Value\x12\x0e\n\x04\x64\x65sc\x18\x01 \x01(\tH\x00\x12\x32\n\x06tensor\x18\x02 \x01(\x0b\x32 .luojianet_ms.prntpb.TensorProtoH\x00\x42\x07\n\x05value'
)




_TENSORPROTO = _descriptor.Descriptor(
  name='TensorProto',
  full_name='luojianet_ms.prntpb.TensorProto',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='dims', full_name='luojianet_ms.prntpb.TensorProto.dims', index=0,
      number=1, type=3, cpp_type=2, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='tensor_type', full_name='luojianet_ms.prntpb.TensorProto.tensor_type', index=1,
      number=2, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='tensor_content', full_name='luojianet_ms.prntpb.TensorProto.tensor_content', index=2,
      number=3, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value=b"",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=36,
  serialized_end=108,
)


_PRINT_VALUE = _descriptor.Descriptor(
  name='Value',
  full_name='luojianet_ms.prntpb.Print.Value',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='desc', full_name='luojianet_ms.prntpb.Print.Value.desc', index=0,
      number=1, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=b"".decode('utf-8'),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='tensor', full_name='luojianet_ms.prntpb.Print.Value.tensor', index=1,
      number=2, type=11, cpp_type=10, label=1,
      has_default_value=False, default_value=None,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
    _descriptor.OneofDescriptor(
      name='value', full_name='luojianet_ms.prntpb.Print.Value.value',
      index=0, containing_type=None,
      create_key=_descriptor._internal_create_key,
    fields=[]),
  ],
  serialized_start=169,
  serialized_end=253,
)

_PRINT = _descriptor.Descriptor(
  name='Print',
  full_name='luojianet_ms.prntpb.Print',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='value', full_name='luojianet_ms.prntpb.Print.value', index=0,
      number=1, type=11, cpp_type=10, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[_PRINT_VALUE, ],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=111,
  serialized_end=253,
)

_PRINT_VALUE.fields_by_name['tensor'].message_type = _TENSORPROTO
_PRINT_VALUE.containing_type = _PRINT
_PRINT_VALUE.oneofs_by_name['value'].fields.append(
  _PRINT_VALUE.fields_by_name['desc'])
_PRINT_VALUE.fields_by_name['desc'].containing_oneof = _PRINT_VALUE.oneofs_by_name['value']
_PRINT_VALUE.oneofs_by_name['value'].fields.append(
  _PRINT_VALUE.fields_by_name['tensor'])
_PRINT_VALUE.fields_by_name['tensor'].containing_oneof = _PRINT_VALUE.oneofs_by_name['value']
_PRINT.fields_by_name['value'].message_type = _PRINT_VALUE
DESCRIPTOR.message_types_by_name['TensorProto'] = _TENSORPROTO
DESCRIPTOR.message_types_by_name['Print'] = _PRINT
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

TensorProto = _reflection.GeneratedProtocolMessageType('TensorProto', (_message.Message,), {
  'DESCRIPTOR' : _TENSORPROTO,
  '__module__' : 'print_pb2'
  # @@protoc_insertion_point(class_scope:luojianet_ms.prntpb.TensorProto)
  })
_sym_db.RegisterMessage(TensorProto)

Print = _reflection.GeneratedProtocolMessageType('Print', (_message.Message,), {

  'Value' : _reflection.GeneratedProtocolMessageType('Value', (_message.Message,), {
    'DESCRIPTOR' : _PRINT_VALUE,
    '__module__' : 'print_pb2'
    # @@protoc_insertion_point(class_scope:luojianet_ms.prntpb.Print.Value)
    })
  ,
  'DESCRIPTOR' : _PRINT,
  '__module__' : 'print_pb2'
  # @@protoc_insertion_point(class_scope:luojianet_ms.prntpb.Print)
  })
_sym_db.RegisterMessage(Print)
_sym_db.RegisterMessage(Print.Value)


# @@protoc_insertion_point(module_scope)
