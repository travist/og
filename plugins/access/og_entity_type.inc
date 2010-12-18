<?php
// $Id: node_type.inc,v 1.12 2010/10/11 22:18:22 sdboyer Exp $

/**
 * @file
 * Plugin to provide access control based upon group's entity type.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("OG: entity type"),
  'description' => t("Control access by group's entity type."),
  'callback' => 'og_entity_type_ctools_access_check',
  'default' => array('type' => array()),
  'settings form' => 'og_entity_type_ctools_access_settings',
  'settings form submit' => 'og_entity_type_ctools_access_settings_submit',
  'summary' => 'og_entity_type_ctools_access_summary',
  'required context' => new ctools_context_required(t('OG group'), 'og_group'),
  'restrictions' => 'og_entity_type_ctools_access_restrictions',
);

/**
 * Settings form for the 'by node_type' access plugin
 */
function og_entity_type_ctools_access_settings($form, &$form_state, $conf) {
  $options = array();
  foreach (entity_get_info() as $type => $info) {
    if (!empty($info['fieldable'])) {
      $options[$type] = check_plain($info['label']);
    }
  }

  $form['settings']['type'] = array(
    '#title' => t('Group\'s entity type'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => t('Only the checked entity types will be valid.'),
    '#default_value' => $conf['type'],
  );
  return $form;
}

/**
 * Compress the entity types allowed to the minimum.
 */
function og_entity_type_ctools_access_settings_submit($form, &$form_state) {
  $form_state['values']['settings']['type'] = array_filter($form_state['values']['settings']['type']);
}

/**
 * Check for access.
 */
function og_entity_type_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || empty($context->data->entity_type)) {
    return FALSE;
  }

  if (array_filter($conf['type']) && empty($conf['type'][$context->data->entity_type])) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Inform the UI that we've eliminated a bunch of possibilities for this
 * context.
 */
function og_entity_type_ctools_access_restrictions($conf, &$context) {
  if (isset($context->restrictions['type'])) {
    $context->restrictions['type'] = array_unique(array_merge($context->restrictions['type'], array_keys(array_filter($conf['type']))));
  }
  else {
    $context->restrictions['type'] = array_keys(array_filter($conf['type']));
  }
}

/**
 * Provide a summary description based upon the checked node_types.
 */
function og_entity_type_ctools_access_summary($conf, $context) {
  if (!isset($conf['type'])) {
    $conf['type'] = array();
  }
  
  $options = array();
  foreach (entity_get_info() as $type => $info) {
    if (!empty($info['fieldable'])) {
      $options[$type] = check_plain($info['label']);
    }
  }  

  $names = array();
  foreach (array_filter($conf['type']) as $type) {
    $names[] = $options[$type];
  }

  if (empty($names)) {
    return t('@identifier is any node type', array('@identifier' => $context->identifier));
  }

  return format_plural(count($names), '@identifier is type "@types"', '@identifier type is one of "@types"', array('@types' => implode(', ', $names), '@identifier' => $context->identifier));
}