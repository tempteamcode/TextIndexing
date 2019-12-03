import { Component, OnInit, AfterViewInit,ViewChild, ElementRef } from '@angular/core';
import { ResearchResults,ResearchItem } from "../../models/research-results";

import { IfImporterService } from "../../services/ifimporter.service";


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

  constructor( private ifPrvd: IfImporterService ) { }

  ngOnInit() {
  }

  ngAfterViewInit() {
      var elems = this.searchTypeSelector.nativeElement
      this.materializeSelect = M.FormSelect.init(elems, {});
  }

  search() {

    this.ifPrvd.searchWords( this.searchWords.split(" ") )
      .subscribe(
        (res) => {
          console.log( res )
        },
        (err) => {
          console.error( err )
        }
      )
    // console.log( this.materializeSelect.getSelectedValues() )
    this.researchResults = new ResearchResults()
    this.researchResults.results.push( new ResearchItem( 5439535, 8 ) )
    this.researchResults.results.push( new ResearchItem( 54, 3 ) )
  }
}
