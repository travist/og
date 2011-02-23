<?php
// $Id$

/**
 * @file
 * Reimplement Drupal 7 API functions as needed.
 */
if (!function_exists('drupal_static')) {
  /**
   * Central static variable storage.
   *
   * @param $name
   *   Globally unique name for the variable. For a function with only one static,
   *   variable, the function name (e.g. via the PHP magic __FUNCTION__ constant)
   *   is recommended. For a function with multiple static variables add a
   *   distinguishing suffix to the function name for each one.
   * @param $default_value
   *   Optional default value.
   * @param $reset
   *   TRUE to reset a specific named variable, or all variables if $name is NULL.
   *   Resetting every variable should only be used, for example, for running
   *   unit tests with a clean environment. Should be used only though via
   *   function drupal_static_reset() and the return value should not be used in
   *   this case.
   *
   * @return
   *   Returns a variable by reference.
   *
   * @see drupal_static_reset()
   */
  function &drupal_static($name, $default_value = NULL, $reset = FALSE) {
    static $data = array(), $default = array();
    // First check if dealing with a previously defined static variable.
    if (isset($data[$name]) || array_key_exists($name, $data)) {
      // Non-NULL $name and both $data[$name] and $default[$name] statics exist.
      if ($reset) {
        // Reset pre-existing static variable to its default value.
        $data[$name] = $default[$name];
      }
      return $data[$name];
    }
    // Neither $data[$name] nor $default[$name] static variables exist.
    if (isset($name)) {
      if ($reset) {
        // Reset was called before a default is set and yet a variable must be
        // returned.
        return $data;
      }
      // First call with new non-NULL $name. Initialize a new static variable.
      $default[$name] = $data[$name] = $default_value;
      return $data[$name];
    }
    // Reset all: ($name == NULL). This needs to be done one at a time so that
    // references returned by earlier invocations of drupal_static() also get
    // reset.
    foreach ($default as $name => $value) {
      $data[$name] = $value;
    }
    // As the function returns a reference, the return should always be a
    // variable.
    return $data;
  }

  /**
   * Reset one or all centrally stored static variable(s).
   *
   * @param $name
   *   Name of the static variable to reset. Omit to reset all variables.
   */
  function drupal_static_reset($name = NULL) {
    drupal_static($name, NULL, TRUE);
  }
}
