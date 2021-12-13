(function () {
  Backdrop.behaviors.data = {
    attach: function attach(context, settings) {
      var data = JSON.stringify(settings.data);
			//
			//This is custom code...
			console.log(data);
			//
    }
  };
})();
