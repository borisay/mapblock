<?php

/**
 * @file
 * Default theme implementation to display page block content.
 *
 * Available variables:
 * - $content: Block content.
 * - $wrapper_classes: Block content wrapper class(es).
 *
 * @see template_preprocess_block()
 */
?>

<div class="<?php print $wrapper_classes;?>">
<?php if ($content): ?>
  <div class="block-body">
    <?php print $content ?>
  </div>
<?php endif; ?>
</div>