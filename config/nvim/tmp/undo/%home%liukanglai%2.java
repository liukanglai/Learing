Vim�UnDo� pg��[�3�o�j��=��wx���o�=�B3    �           G                       b��0    _�                             ����                                                                                                                                                                                                                                                                                                                                                             b��.     �               �               �               �               �                  5��                                                  �                                         
       �                                                �                          ;                      �                          J                      5�_�                             ����                                                                                                                                                                                                                                                                                                                                                             b��/    �   �            �   �   �          +                tcellStyle.setFontSize(13);�   �   �          /                tcellStyle.setForeColor("Red");�   �   �          !                //设置前景色�   �   �          %                tcellStyle.setRow(i);�   �   �                          //设置行号�   �   �          :                tcellStyle.setFieldKey("ilou_datefield1");�   �   �          !                //设置字段名�   �   �          a                kd.bos.entity.report.CellStyle tcellStyle = new kd.bos.entity.report.CellStyle();�   �   �           �   �   �          *                cellStyles.add(cellStyle);�   �   �          *                cellStyle.setFontSize(13);�   �   �          .                cellStyle.setForeColor("Red");�   �   �          !                //设置前景色�   �   �          $                cellStyle.setRow(i);�   �   �                          //设置行号�   �   �          8                cellStyle.setFieldKey("ilou_textfield");�   �   �          `                kd.bos.entity.report.CellStyle cellStyle = new kd.bos.entity.report.CellStyle();�   �   �                      }else{�   �   �                          �   �   �          +                cellStyles.add(tcellStyle);�   �   �          +                tcellStyle.setFontSize(13);�   �   �          1                tcellStyle.setForeColor("Green");�   �   �          !                //设置前景色�   �   �          %                tcellStyle.setRow(i);�   �   �                          //设置行号�   �   �          :                tcellStyle.setFieldKey("ilou_datefield1");�   �   �          !                //设置字段名�   �   �          a                kd.bos.entity.report.CellStyle tcellStyle = new kd.bos.entity.report.CellStyle();�   �   �           �   �   �          *                cellStyles.add(cellStyle);�   �   �          *                cellStyle.setFontSize(13);�   �   �          0                cellStyle.setForeColor("Green");�   �   �          !                //设置前景色�   �   �          $                cellStyle.setRow(i);�   �   �                          //设置行号�   �   �          8                cellStyle.setFieldKey("ilou_textfield");�   �   �          !                //设置字段名�      �          `                kd.bos.entity.report.CellStyle cellStyle = new kd.bos.entity.report.CellStyle();�   ~   �                      {�   }                        if(result<0)�   |   ~          ,           // if("待还".equals(exeResult))�   {   }          e            //这里将值为success的字段的颜色设置为绿色,failure的字段设置为红色�   z   |          B            String exeResult = object.getString("ilou_textfield");�   y   {          ,            //获取要判断的状态字段�   x   z           �   w   y          '		    int result=today.compareTo(time);�   v   x          /            //比较当前时间与截止日期�   u   w          d		    String today= new SimpleDateFormat("yyyy-MM-dd").format(new Date(System.currentTimeMillis()));�   t   v          >            String time = object.getString("ilou_datefield1");�   s   u          =            String state=object.getString("ilou_datefield1");�   r   t          ,            //获取要判断的时间字段�   q   s          n            DynamicObject object = BusinessDataServiceHelper.loadSingle(primaryKeyValue, "ilou_borrow__book");�   p   r          >            Object primaryKeyValue = row.getPrimaryKeyValue();�   o   q          "            //获取行数据的PK�   n   p          1            ListSelectedRow row = allList.get(i);�   m   o          *        for(int i=0;i<allList.size();i++){�   l   n          P        ArrayList<kd.bos.entity.report.CellStyle> cellStyles= new ArrayList<>();�   k   m          V        ListSelectedRowCollection allList = billList.getCurrentListAllRowCollection();�   j   l          A        //获取当前页列表的所有行,下一页获取不到�   i   k          :        BillList billList = this.getControl("billlistap");�   h   j          ?    	//固定写法,获取列表控件都使用这个billlistap�   g   i          .    public void afterBindData(EventObject e) {�   f   h              @Override�   e   g              //加载数据后触发�   d   f              �   c   e           �   b   d              �   a   c              }�   `   b          	        }�   _   a           �   ^   `          I            this.getView().showSuccessNotification("书籍归还成功");�   ]   _          6            this.getView().invokeOperation("refresh");�   \   ^                     �   [   ]          )            billList.refreshData();      �   Z   \                      */�   Y   [          6            DeleteServiceHelper.delete(type,pks);     �   X   Z          5            EntityType type=billList.getEntityType();�   W   Y          9            /*Object[] pks=allList.getPrimaryKeyValues();�   V   X          9	            //获取选中的几条数据的pk,并删除�   U   W           �   T   V           �   S   U                      }*/�   R   T           �   Q   S          V                DeleteServiceHelper.delete("kdec_application",new QFilter[]{qFilter});�   P   R          [                QFilter qFilter = new QFilter("billno",QFilter.equals,"A-20211124-000014");�   O   Q           �   N   P          r                DynamicObject object = BusinessDataServiceHelper.loadSingle(primaryKeyValue, "ilou_borrow__book");�   M   O          B                Object primaryKeyValue = row.getPrimaryKeyValue();�   L   N          5                ListSelectedRow row = allList.get(i);�   K   M           �   J   L          -           for(int i=0;i<allList.size();i++){�   I   K                      /*�   H   J                     �   G   I                     �   F   H                  �   E   G           �   D   F                      }�   C   E          o                SaveServiceHelper.saveOperate("ilou_borrow__book", new DynamicObject[] {object}, null);//保存�   B   D          ,                object.set("", "已归还");�   A   C          3                //修改选中借书记录的状态�   @   B                          �   ?   A          t                DynamicObject object = BusinessDataServiceHelper.loadSingle(primaryKeyValue, "ilou_borrow__book");  �   >   @          B                Object primaryKeyValue = row.getPrimaryKeyValue();�   =   ?          &                //获取行数据的PK�   <   >          5                ListSelectedRow row = allList.get(i);�   ;   =                      {�   :   <          -            for(int i=0;i<allList.size();i++)�   9   ;          T            ArrayList<kd.bos.entity.report.CellStyle> cellStyles= new ArrayList<>();�   8   :          7           /**************************************** */�   7   9                     �   6   8          K            ListSelectedRowCollection allList = billList.getSelectedRows();�   5   7          ;            //获取当前勾选了的全部行数据详情�   4   6          >            BillList billList = this.getControl("billlistap");�   3   5           �   2   4                      e.setCancel(true);�   1   3          =            this.getView().showMessage("点击归还按钮");�   0   2          *            // 取消修改操作的执行�   /   1          	        {�   .   0          -        if (buttonID.equals( e.getItemKey()))�   -   /              {�   ,   .          7    public void beforeItemClick(BeforeItemClickEvent e)�   +   -           �   *   ,          9    public final static String buttonID="ilou_baritemap";�   )   +           �   (   *          {�   '   )          8public class BookBillPlugln  extends AbstractListPlugin �   &   (           �   %   '           �   $   &          8import kd.bos.servicehelper.operation.SaveServiceHelper;�   #   %          :import kd.bos.servicehelper.operation.DeleteServiceHelper;�   "   $          6import kd.bos.servicehelper.BusinessDataServiceHelper;�   !   #           import kd.bos.orm.query.QFilter;�       "          -import kd.bos.list.plugin.AbstractListPlugin;�      !          import kd.bos.list.IListView;�                 import kd.bos.list.BillList;�                7import kd.bos.form.control.events.BeforeItemClickEvent;�                %import kd.bos.form.control.EntryGrid;�                #import kd.bos.form.control.Control;�                import kd.bos.form.IFormView;�                9import kd.bos.entity.datamodel.ListSelectedRowCollection;�                /import kd.bos.entity.datamodel.ListSelectedRow;�                *import kd.bos.entity.datamodel.IDataModel;�                 import kd.bos.entity.EntityType;�                8import kd.bos.dataentity.entity.DynamicObjectCollection;�                .import kd.bos.dataentity.entity.DynamicObject;�                import kd.bos.bill.IBillPlugin;�                &import kd.bos.bill.AbstractBillPlugIn;�                import antlr.collections.List;�                import antlr.StringUtils;�                 �                5import com.kingdee.bos.ctrl.kdf.tablepanel.CellStyle;�                 �                import java.util.EventObject;�                import java.util.Date;�   
             import java.util.ArrayList;�   	             "import java.text.SimpleDateFormat;�      
           �      	          #package kd.bos.debug.mservice.Plug;�                 �                 �                // Version:V1.0�                // Name:2.java�                // Time:2022-07-06 09:45:18�                	// Blog: �                 // Author:liukanglai�   �   �          +                cellStyles.add(tcellStyle);               }       	        }   *        billList.setCellStyle(cellStyles);       }   }�   q              G            DynamicObject object = BusinessDataServiceHelper.loadSingle�   r            �                 // �               �               5��                          Z                      �                          Z                      �                   l   G   Z               �      �    q   G           :      S              �	      �    �                      �      x               �                                                �               	                 	              �                                              �                         :                     �                         I                     �                           Y                       �                       #   Z               #       �               #           ~       #               �                                                �    	           "          �       "              �    
                  5   �              5       �                      "   �              "       �                                             �                          -                     �               5          D      5              �                       &   b              &       �                         �                    �                      .   �             .       �               &       8   �      &       8       �                                               �               .       *   2      .       *       �               8       /   ]      8       /       �                       9   �              9       �               *          �      *              �               /       #   �      /       #       �               9       %   	      9       %       �                      7   /             7       �               #          g      #              �               %          �      %              �               7       -   �      7       -       �                          �                     �                      6   �             6       �                -       :   (      -       :       �    !                   8   c              8       �    "           6           �      6               �    #           :       8   �      :       8       �    $           8           �      8               �    %                   9   �              9       �    &                                             �    '           8       7         8       7       �    (                  *   J             *       �    )                   $   u              $       �    *           9       7   �      9       7       �    +                      �                     �    ,           7           �      7               �    -                  8   �             8       �    .           -       5   %      -       5       �    /           	       E   [      	       E       �    0           *           �      *               �    1           =       2   �      =       2       �    2                  O   �             O       �    3                   0   %              0       �    4           >       -   V      >       -       �    5           ;          �      ;              �    6           K       :   �      K       :       �    7                  D   �             D       �    8           7       2   #      7       2       �    9           T           V      T               �    :           -       +   W      -       +       �    ;                  $   �             $       �    <           5       &   �      5       &       �    =           &       N   �      &       N       �    >           B          	      B              �    ?           t           &	      t               �    @                     '	                    �    A           3       '   0	      3       '       �    B           ,           X	      ,               �    C           o       /   Y	      o       /       �    D                  <   �	             <       �    E                       �	                      �    F                  P   �	             P       �    G                      8
                     �    H                     9
                    �    I                  :   Y
             :       �    J           -       P   �
      -       P       �    K                       �
                      �    L           5       	   �
      5       	       �    M           B           �
      B               �    N           r       2   �
      r       2       �    O                   3   $              3       �    P           [       /   X      [       /       �    Q           V       +   �      V       +       �    R                      �                     �    S                     �                    �    T                       �                      �    U                   0   �              0       �    V           9       C         9       C       �    W           9          Q      9              �    X           5          W      5              �    Y           6           [      6               �    Z                     \                    �    [           )          v      )              �    \                  ,   �             ,       �    ]           6       >   �      6       >       �    ^           I       6   �      I       6       �    _                   =   %              =       �    `           	       '   c      	       '       �    a                  2   �             2       �    b                  M   �             M       �    c                   .                 .       �    d                  +   ;             +       �    e                     g                    �    f                  8   �             8       �    g           .       B   �      .       B       �    h           ?       0          ?       0       �    i           :       &   1      :       &       �    j           A       9   X      A       9       �    k           V       8   �      V       8       �    l           P       7   �      P       7       �    m           *       G         *       G       �    n           1       )   K      1       )       �    o           "       )   u      "       )       �    p           >           �      >               �    q           n       &   �      n       &       �    r           ,       <   �      ,       <       �    s           =       _         =       _       �    t           >       '   d      >       '       �    u           d          �      d              �    v           /       2   �      /       2       �    w           '       1   �      '       1       �    x                      	                     �    y           ,       0   #      ,       0       �    z           B          T      B              �    {           e          k      e              �    |           ,          �      ,              �    }                  (   �             (       �    ~                  "   �             "       �               `       "   �      `       "       �    �           !                 !               �    �           8       3         8       3       �    �                  1   F             1       �    �           $          x      $              �    �           !       2   �      !       2       �    �           0          �      0              �    �           *          �      *              �    �           *          �      *              �    �                   )                 )       �    �           a       #   >      a       #       �    �           !       #   b      !       #       �    �           :           �      :               �    �                     �                    �    �           %       2   �      %       2       �    �           !       1   �      !       1       �    �           1       0   �      1       0       �    �           +          ,      +              �    �           +          C      +              �    �                     `                    �    �                  &   z             &       �    �           `       "   �      `       "       �    �           8       "   �      8       "       �    �                      �                     �    �           $       3   �      $       3       �    �           !       1         !       1       �    �           .          N      .              �    �           *       2   h      *       2       �    �           *          �      *              �    �                      �                     �    �           a          �      a              �    �           !       '   �      !       '       �    �           :       #         :       #       �    �                  #   6             #       �    �           %          Z      %              �    �           !          b      !              �    �           /       &   h      /       &       �    �           +          �      +              �    �                      �                     5��