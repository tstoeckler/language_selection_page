<?php
/**
 * @file
 * The admin page of the language selection page module.
 */


/**
 * The admin page form.
 *
 * @return array
 */
function i18n_selection_page_admin() {

  $form = array();
  $error = FALSE;

  if (variable_get('language_negotiation', LANGUAGE_NEGOTIATION_NONE) != 2) {
    drupal_set_message(t('The Language Selection Page settings are not available until %setting_name setting is not set to required "%setting_value" value. You should either disable the module or <a href="@setting_url">change your configuration</a>.', array(
                                                                                                                                                                                                                                                                 '%setting_name' => t('Language negotiation'),
                                                                                                                                                                                                                                                                 '%setting_value' => t('Path prefix with language fallback.'),
                                                                                                                                                                                                                                                                 '@setting_url' => url('admin/settings/language/configure'),
                                                                                                                                                                                                                                                            )), 'warning');
    $error = TRUE;
  }

  $lang_list = language_list('enabled');
  $lang_list = $lang_list[1];

  if (count($lang_list) < 2) {
    drupal_set_message(t('You should have more than <a href="@setting_url">one language</a> enabled to get the Language Selection Page working.', array(
                                                                                                                                                       '@setting_url' => url('admin/settings/language'),
                                                                                                                                                  )), 'warning');
    $error = TRUE;
  }

  if ($error) return $form;

  $options = array('1' => t('No but redirect if language is found from cookie.'), '2' => t('No'), '4' => t('Yes'));
  $form['i18n_selection_page_enable'] = array(
    '#title' => t('Redirect to a language selection page if no language is detected from URL and/or Cookie ?'),
    '#type' => 'radios',
    '#default_value' => variable_get('i18n_selection_page_enable', 2),
    '#options' => $options,
    '#description' => t('Select yes if you want to enable the language selection page when no language is detected from URL and/or Cookie.')
  );

  $options = array('8' => t('No'), '16' => t('Yes'));
  $form['i18n_selection_page_use_language_cookie'] = array(
    '#title' => t('Use a cookie to remember your language ?'),
    '#type' => 'radios',
    '#default_value' => variable_get('i18n_selection_page_use_language_cookie', 8),
    '#options' => $options,
    '#description' => t('Select yes if you want to store the language in a cookie.<br/>The cookie is <i>' . LANGUAGE_COOKIE_KEY . '</i>')
  );

  $options = array('32' => t('Template in theme'), '64' => 'Template only');
  $form['i18n_selection_page_redirect_type'] = array(
    '#title' => t('Select the way the Selection Page should work'),
    '#type' => 'select',
    '#multiple' => FALSE,
    '#default_value' => variable_get('i18n_selection_page_redirect_type', 64),
    '#options' => $options,
    '#description' => t('<b>Template in theme</b>: Insert the Language Selection Page in the template in the current theme.<br/><b>Template only</b>: Display the Selection Page template only.<br/>Create a file named <i>i18n_selection_page.tpl.php</i> in your theme directory if you want to override it.')
  );

  $form['i18n_selection_page_blacklisted_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('List of paths to blacklist'),
    '#default_value' => implode("\n", variable_get('i18n_selection_page_blacklisted_paths', array('admin', 'user', 'admin/*', 'admin*', 'node/add/*', 'node/*/edit'))),
    '#size' => 10,
    '#description' => t('Write on each line a path to blacklist from Language Selection Page processor')
  );

  $form['i18n_selection_page_cookie_lifetime'] = array(
    '#type' => 'textfield',
    '#field_suffix' => t('Seconds'),
    '#title' => t('Cookie lifetime'),
    '#default_value' => variable_get('i18n_selection_page_cookie_lifetime', 2592000),
    '#size' => 10,
    '#description' => t('Cookie lifetime, must be greater than zero. (2592000 = 1 month)')
  );

  $form['#submit'][] = 'i18n_selection_page_admin_submit';

  return system_settings_form($form);

}

function i18n_selection_page_admin_validate($form, &$form_state) {
  $form_state['values']['i18n_selection_page_blacklisted_paths'] = explode("\n", trim($form_state['values']['i18n_selection_page_blacklisted_paths']));

  if ($form_state['values']['i18n_selection_page_enable'] == 1 && $form_state['values']['i18n_selection_page_use_language_cookie'] == 8) {
    form_set_error('i18n_selection_page_use_language_cookie', t('Enable cookie if you want to use cookie.'));
  }

  if ($form_state['values']['i18n_selection_page_cookie_lifetime'] <= 0) {
    form_set_error('i18n_selection_page_cookie_lifetime', t('The cookie lifetime must be greater than zero.'));
  }
}

function i18n_selection_page_admin_submit($form, &$form_state) {
  if ($form_state['values']['i18n_selection_page_use_language_cookie'] == 8) {
    setcookie(LANGUAGE_COOKIE_KEY, NULL, 0, '/');
    drupal_set_message(t('I18N Selection Page Cookie removed.'));
  }
}