#!/bin/bash

readonly source_dir="${THIRD_PARTY_ROOT}/source"
readonly install_dir="${THIRD_PARTY_ROOT}/install/${DIST}"
readonly build_dir="${THIRD_PARTY_ROOT}/build/${DIST}"


_execute_steps_on()
{
  local module_name="${1}"
  shift
  local steps="${@}"
  for fcn in "${@}"; do
    echo "${module_name}_${fcn}"
    ${module_name}_${fcn} || break
  done
}


main_for()
{
  local module_name="${1}"
  shift
  local positional="${@}"

  if [[ -z "${THIRD_PARTY_ROOT}" ]]; then
    echo "Environment variable THIRD_PARTY_ROOT not set"
    exit 1
  elif [[ -z "${DIST}" ]]; then
    echo "Environment variable DIST not set"
    return
  elif [[ ${#} -eq 0 ]]; then
    _execute_steps_on ${module_name} \
      fetch \
      configure \
      install
  else
    _execute_steps_on ${module_name} \
      "${@}"
  fi
}
