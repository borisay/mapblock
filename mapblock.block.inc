<?php

/**
 * BlockMap extends Block.
 *
 * This class allows us to create blocks with text and map.
 */
class BlockMap extends Block {
  /**
   * Sets title text on draggable block panel in Layout builder.
   */
  public function getAdminTitle() {
    return !empty($this->settings['label']) ? check_plain($this->settings['label']) : t('Map block');
  }

  /**
   * Returns the rendered content of this block.
   *
   * @return string
   *   Rendered markup of an map block content.
   */
  public function getContent() {
    $elements = array(
      'content' => '',
      'wrapper_classes' => 'mapblock-inner',
    );

    $block = config_get('mapblock.block.' . $this->settings['machine_name']);
		$raw_data = views_get_view_result('data','NULL' );
    if (!empty($block['content'])) {
      $content = token_replace($block['content']);
      $elements['content'] = check_markup($content, $block['format'], '', TRUE);
    }
    if (!empty($raw_data)){
      foreach ($raw_data as $i=>$element){
        $elements['data'][$i]['nid'] = $element->nid;
        $elements['data'][$i]['name'] = $element->node_title;
        $elements['data'][$i]['active'] = $element->field_field_active[0]['raw']['value'];
        $elements['data'][$i]['start_date'] = strtok($element->field_field_start_date[0]['raw']['value'], ' ');
        $elements['data'][$i]['end_date'] = strtok($element->field_field_end_date[0]['raw']['value'], ' ');
      }
    }
    backdrop_add_js(array('data' => array('nodes' => $elements['data'])), 'setting');
    return theme('mapblock_content', $elements);
  }

  /**
   * Builds the block's configuration form.
   */
  public function form(&$form, &$form_state) {
    parent::form($form, $form_state);
    if ($this->is_new) {
      $args = arg();
      if (isset($args[8])) {
        $block_info = explode(':', $args[8]);
        if (isset($block_info[1]) && $block_info[1] != 'default') {
          $block = config_get('mapblock.block.' . $block_info[1]);
          $this->settings['machine_name'] = '';
          $this->settings['title'] = $block['title'];
        }
      }
    }
    else {
      $block = config_get('mapblock.block.' . $this->settings['machine_name']);
    }
    $form['machine_name'] = array(
      '#type' => 'machine_name',
      '#default_value' => $this->settings['machine_name'],
      '#maxlength' => 128,
      '#machine_name' => array(
        'exists' => 'mapblock_load',
        'source' => array('label'),
      ),
      '#description' => t('A unique machine-readable name for this Block. It must only contain lowercase letters, numbers, and underscores.'),
      '#disabled' => !empty($this->settings['machine_name']),
    );

    $form['content'] = array(
      '#type' => 'text_format',
      '#title' => t('Block content'),
      '#default_value' => isset($block['content']) ? $block['content'] : '',
      '#format' => isset($block['format']) ? $block['format'] : NULL,
      '#editor_uploads' => TRUE,
      '#rows' => 5,
    );

  }

  /**
   * Submit handler to save the form settings.
   */
  public function formSubmit($form, &$form_state) {
    parent::formSubmit($form, $form_state);
    $this->settings['label'] = $form_state['values']['label'];
    $this->settings['machine_name'] = $form_state['values']['machine_name'];
    $block = array();
    $block['parent'] = $this->delta;
    $block['label'] = $form_state['values']['label'];
    $block['machine_name'] = $form_state['values']['machine_name'];
    $block['title'] = $form_state['values']['title'];
    $block['content'] = $form_state['values']['content']['value'];
    $block['format'] = $form_state['values']['content']['format'];
    
    $config = config('mapblock.block.' . $block['machine_name']);
    $config->setData($block);
    $config->save();
  }
}
