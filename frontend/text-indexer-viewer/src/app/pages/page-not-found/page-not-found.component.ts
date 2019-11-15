import { Component, OnInit, AfterViewInit, ViewChild, ElementRef } from '@angular/core';

@Component({
  selector: 'app-page-not-found',
  templateUrl: './page-not-found.component.html',
  styleUrls: ['./page-not-found.component.css']
})
export class PageNotFoundComponent implements OnInit, AfterViewInit {

  @ViewChild("snowcanvas")
  private snowCanvas: ElementRef

  constructor() { }

  ngOnInit() {
  }

  ngAfterViewInit() {
    	let makeSnow = (el) => {
    		var ctx = el.getContext('2d');
    		var width = 0;
    		var height = 0;
    		var particles = [];

    		var Particle = function() {
    			this.x = this.y = this.dx = this.dy = 0;
    			this.reset();
    		}

    		Particle.prototype.reset = function() {
    			this.y = Math.random() * height;
    			this.x = Math.random() * width;
    			this.dx = (Math.random() * 1) - 0.5;
    			this.dy = (Math.random() * 0.5) + 0.5;
    		}

    		let createParticles = (count) => {
    			if (count != particles.length) {
    				particles = [];
    				for (var i = 0; i < count; i++) {
    					particles.push(new Particle());
    				}
    			}
    		}

    		let onResize = () => {
    			width = window.innerWidth;
    			height = window.innerHeight;
    			el.width = width;
    			el.height = height;

    			createParticles((width * height) / 10000);
    		}

    		let updateParticles = () => {
    			ctx.clearRect(0, 0, width, height);
    			ctx.fillStyle = '#f6f9fa';

    			particles.forEach(function(particle) {
    				particle.y += particle.dy;
    				particle.x += particle.dx;

    				if (particle.y > height) {
    					particle.y = 0;
    				}

    				if (particle.x > width) {
    					particle.reset();
    					particle.y = 0;
    				}

    				ctx.beginPath();
    				ctx.arc(particle.x, particle.y, 5, 0, Math.PI * 2, false);
    				ctx.fill();
    			});

    			window.requestAnimationFrame(updateParticles);
    		}

    		onResize();
    		updateParticles();

    		window.addEventListener('resize', onResize);
    	}

  		var canvas = this.snowCanvas.nativeElement
  		makeSnow(canvas);

  }

}
