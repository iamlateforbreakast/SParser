import paper, { view } from 'paper';

var point = new Point(20, 150);
var size = new Size(100, 50);
var myRectangle = new Path.Rectangle(point, size);
myRectangle.fillColor = 'powderblue';

console.log('window onload');
window.onload = function() {
	// paper.setup(CANVAS);
	// paper.install(window);

	view.onFrame = animate;
	// init();
};

//const init = () => {
// 	console.log('init called');
// render_curves();
// };



function animate(event) {
  myRectangle.rotate(3);
}
