<?php

/**
 * @file
 * Phing-related drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function phing_drush_command() {
  $items = array();

  $items['phing-list-modules'] = array(
    'description' => 'List all custom modules in a site.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  $items['phing-list-tests'] = array(
    'description' => 'List all test cases made available by custom modules.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function phing_drush_help($section) {
  switch ($section) {
    case 'meta:phing:title':
      return dt('Phing');
    case 'meta:phing:summary':
      return dt('Phing-related drush commands.');
    case 'drush:phing-list-modules':
      return dt('List all test cases made available by custom modules.');
      case 'drush:phing-list-tests':
        return dt('List all test cases made available by custom modules.');
  }
}

/**
 * List modules command.
 */
function drush_phing_list_modules() {
  $modules = db_query("SELECT name FROM system WHERE type = 'module' AND filename LIKE 'sites/all/modules/custom/%.module'")->fetchCol();

  foreach ($modules as $module) {
    drush_print($module);
  }
}

/**
 * List tests command.
 */
function drush_phing_list_tests() {
  $classes = db_query("SELECT name FROM registry WHERE type = 'class' AND filename LIKE 'sites/all/modules/custom/%.test'")->fetchCol();

  $groups = array();
  foreach ($classes as $class) {
    if (class_exists($class) && method_exists($class, 'getInfo')) {
      if (class_exists($class) && method_exists($class, 'getInfo')) {
        $info = call_user_func(array($class, 'getInfo'));

        if (!empty($info['dependencies'])) {
          foreach ($info['dependencies'] as $module) {
            if (!drupal_get_filename('module', $module)) {
              continue 2;
            }
          }
        }

        $groups[$info['group']]++;
      }
    }
  }

  if (!empty($groups)) {
    foreach (array_keys($groups) as $group_name) {
      drush_print('Simpletest: ' . $group_name);
    }
  }
}
