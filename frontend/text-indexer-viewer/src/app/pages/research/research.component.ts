import { Component, OnInit, AfterViewInit,ViewChild, ElementRef } from '@angular/core';
import { ResearchResults,ResearchItem } from "../../models/research-results";


declare var M

@Component({
  selector: 'app-research',
  templateUrl: './research.component.html',
  styleUrls: ['./research.component.css']
})
export class ResearchComponent implements OnInit {


  @ViewChild('searchTypeSelector')
  public searchTypeSelector: ElementRef
  private materializeSelect: any = null

  public searchWords: string = ""
  public searchType: number = -1

  public researchResults: ResearchResults = null

  constructor() { }

  ngOnInit() {
  }

  ngAfterViewInit() {
      var elems = this.searchTypeSelector.nativeElement
      this.materializeSelect = M.FormSelect.init(elems, {});
  }

  search() {
    // console.log( this.materializeSelect.getSelectedValues() )
    this.researchResults = new ResearchResults()
    this.researchResults.results.push( new ResearchItem( 5439535, 8 ) )
    this.researchResults.results.push( new ResearchItem( 54, 3 ) )
  }
}
