import { TestBed, inject } from '@angular/core/testing';

import { IfimporterService } from './ifimporter.service';

describe('IfimporterService', () => {
  beforeEach(() => {
    TestBed.configureTestingModule({
      providers: [IfimporterService]
    });
  });

  it('should be created', inject([IfimporterService], (service: IfimporterService) => {
    expect(service).toBeTruthy();
  }));
});
