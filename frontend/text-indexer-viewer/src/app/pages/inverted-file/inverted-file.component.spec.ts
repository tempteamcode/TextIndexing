import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { InvertedFileComponent } from './inverted-file.component';

describe('InvertedFileComponent', () => {
  let component: InvertedFileComponent;
  let fixture: ComponentFixture<InvertedFileComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ InvertedFileComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(InvertedFileComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
