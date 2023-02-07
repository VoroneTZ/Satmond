//Add an effect to a views stage chain
VIEW* sc_ppAdd(MATERIAL* Material,VIEW* View,BMAP* bmap)
{
	//find the last view of "View"s effectchain and store its pointer
	sc_view_last = View;
	while(sc_view_last.stage != NULL)
	{
		sc_view_last = sc_view_last.stage;
	}
	
	//create a new view as the stored views stage
	sc_view_last.stage = view_create(0);
	set(sc_view_last.stage,PROCESS_TARGET);
	
	//assign "Material" to the just created view
	sc_view_last = sc_view_last.stage;
	sc_view_last.material = Material;
	
	//if a bmap is given, render the view into it
	if(bmap != NULL)
	{
		sc_view_last.bmap = bmap;
	}
	
	//return the pointer to the new view
	return(sc_view_last);
}


//remove an effect from a views stage chain
int sc_ppRemove(MATERIAL* Material,VIEW* View,VIEW* StageView)
{
	//find the view with the material selected or "StageView" and the previous view
	sc_view_last = View;
	while(sc_view_last.material != Material && ((StageView == NULL)+(sc_view_last.stage != NULL)) != NULL)
	{
		View = sc_view_last;
		sc_view_last = sc_view_last.stage;
		
		//return one if the stage doesn´t exist
		if(sc_view_last == NULL){return(1);}
	}
	
	//pass the views stage to the previous view
	View.stage = sc_view_last.stage;
	
	//reset the views bmap to null
	sc_view_last.bmap = NULL;
	
	//remove the view
	ptr_remove(sc_view_last);
	
	//return null if everything worked
	return(0);
}