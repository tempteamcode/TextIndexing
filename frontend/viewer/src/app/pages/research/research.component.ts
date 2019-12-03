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

  public researchResults: Map<string,ResearchResults>
    = new Map<string,ResearchResults>()

  constructor( private ifPrvd: IfImporterService ) { }

  getKeys(map){
      return Array.from(map.keys());
  }

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
          this.researchResults.clear();
          console.log( res )
          for( let s in res ) {
            // console.log( s, res[s], res[s]["results"])
            let rs = new ResearchResults()
            for( let i of res[s]["results"] ) {
              // console.log( i )
              rs.results.push(
                    new ResearchItem(
                      i["documentID"],
                      i["count"],
                      i["global_word_count"]
                    )
                  )
            }

            this.researchResults.set( s, rs )

          }
        },
        (err) => {
          console.error( err )
        }
      )
    // console.log( this.materializeSelect.getSelectedValues() )
    // this.researchResults = new ResearchResults()
    // this.researchResults.results.push( new ResearchItem( 5439535, 8 ) )
    // this.researchResults.results.push( new ResearchItem( 54, 3 ) )
  }
}
