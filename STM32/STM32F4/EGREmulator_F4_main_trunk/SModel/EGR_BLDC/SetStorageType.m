%this script is used to achieve these goals:
%   1. make sure the generated code's variable name is equal with the
%      name in model.
%   2. make sure the constant/nD LookUp Tabel's init value is come from init file
%      and we can change the value from outside fo code.

% mode_name:the model we will to process.
function SetStorageType(model_name)

    sys_handle = load_system(model_name);
    set_param(sys_handle, 'SolverName', 'FixedStepDiscrete');
    set_param(sys_handle, 'InlineParams', 'on');
    
    %set inport name on the line of this block
    SetBlockNameOnLine(sys_handle, 'Inport', 'Outport');
    %%set outport name on the line of this block
    SetBlockNameOnLine(sys_handle, 'Outport', 'Inport');
    
    %set RTWStorageClass
    SetBlockRTWStorageClass(sys_handle);
    
    %assign value for constant
    AssignValueForConstant(sys_handle);
    
    %assign value for LookUp
    AssignValueForLookUpTable(sys_handle);
    
    save_system(sys_handle);
end


function SetBlockNameOnLine(sys_handle, blockType, portType)
    all_handles = find_system(sys_handle, 'SearchDepth', '1',  'LookUnderMasks', 'all', 'BlockType', blockType);
    block_num = length(all_handles);
    for i = 1:block_num
        block_handle = all_handles(i);
        block_name = get_param(block_handle, 'name');  
        line_handle = get_param(block_handle, 'LineHandles');
        varname = @(x) inputname(1);
        line_handle_name = varname(line_handle);
        linePortHandle= eval([line_handle_name, '.',  portType]);
        line_name = get_param(linePortHandle, 'name');
        if isempty(line_name)
            set_param(linePortHandle, 'name', block_name);
        end
        
        if(excludePortName(block_name))
             set_param(linePortHandle, 'name', '');
        end
    end
end


%this function is used to set the block's RTWStorageClass to ExportedGlobal
function SetBlockRTWStorageClass(sys_handle)
    portNameStruct = struct;
    all_block_handles = find_system(sys_handle, 'FindAll', 'on', 'LookUnderMasks', 'all', 'type', 'block');
    block_num = length(all_block_handles);
    for i = 1:block_num
        block_handle = all_block_handles(i);
        outputsignals = get_param(block_handle, 'OutputSignalNames');
        outputsignalsNum = length(outputsignals);
        block_type = get_param(block_handle, 'BlockType');
        if 1 ~= outputsignalsNum || isequal(block_type, 'SubSystem')
            continue;
        end
       
        outputsignal = outputsignals{1};
        port_handles = get_param(block_handle, 'portHandles');
        if ~isempty(outputsignal) && ~isfield(portNameStruct, outputsignal)
            try 
                outportHandle = port_handles.Outport;
                signalNameFromLabel = get_param(outportHandle(1), 'SignalNameFromLabel');
                propagatedSignals = get_param(outportHandle(1), 'PropagatedSignals');
                if ~isempty(propagatedSignals)
                    set_param(outportHandle(1), 'RTWStorageClass', 'Auto');
                    continue;
                end
                
                if isempty(signalNameFromLabel)
                    set_param(outportHandle(1), 'SignalNameFromLabel', outputsignal);
                end
                set_param(outportHandle(1), 'RTWStorageClass', 'ExportedGlobal');
                portNameStruct.(outputsignal) = 1;
            catch exception
                blockName = get_param(block_handle, 'Name');
                disp([blockName, ' RTWStorageClass to ExportedGlobal failed!']);
                msgString = getReport(exception);
                disp(msgString);
            end
        end
    end
end



%this function is used to assign the constant's init value from the
%baseworkspace
function AssignValueForConstant(sys_handle)
    all_constant_handles = find_system(sys_handle,  'FindAll', 'on', 'LookUnderMasks', 'all', 'BlockType', 'Constant');
    constant_num = length(all_constant_handles);
    for i = 1:constant_num
        constant_handle = all_constant_handles(i);
        constant_value = get_param(constant_handle, 'Value');
        line_handle = get_param(constant_handle, 'LineHandles');
        set_param(line_handle.Outport, 'name', '');
        if isvarname(constant_value)
            assign_value_tobasespace(constant_value);
        end
    end
end

function assign_value_tobasespace(var)
    %the table's value is fixed that we can't change the value outside by
    %the variable name
    if ~isvarname(var)
        return
    end

    %we can change the table's value outside by the variable name
    try value = evalin('base', var);
        if isa(value, 'Simulink.Parameter')
            return
        else
            size_a = size(value);
            str_value = [];
            
            if size_a == 1
                str_value = num2str(value);
            else
                for i = 1:size_a
                    str_value = strcat(str_value, num2str(value(i,:)), ';');
                end
            end
            
            evalin('base', strcat(var, ' = Simulink.Parameter'));
            evalin('base', strcat(var, '.RTWInfo.StorageClass = ''ExportedGlobal'''));
            evalin('base', strcat(var, '.Value = [', str_value, ']'));
        end
    catch
    end
    
end


%this function is used to assign the nDLookUp's init value from the
%baseworkspace
function AssignValueForLookUpTable(sys_handle)
    %set 2D RTWStorageClass
    all_2D_table_handles = find_system(sys_handle, 'FindAll', 'on', 'LookUnderMasks', 'all', 'BlockType', 'Lookup');
    table_num = length(all_2D_table_handles);
    for i = 1 : table_num
        table_handle = all_2D_table_handles(i);
        XLookUpData = get_param(table_handle, 'XLookUpData');
        assign_value_tobasespace(XLookUpData);
        
        tableData = get_param(table_handle, 'Table');
        assign_value_tobasespace(tableData);
    end
    
    %set 3D RTWStorageClass
    all_3D_table_handles = find_system(sys_handle, 'FindAll', 'on', 'LookUnderMasks', 'all', 'BlockType', 'Lookup2D');
    table_num = length(all_3D_table_handles);
    for i = 1 : table_num
        table_handle = all_3D_table_handles(i);
        rowIndex = get_param(table_handle, 'RowIndex');
        assign_value_tobasespace(rowIndex);
        
        columnIndex = get_param(table_handle, 'columnIndex');
        assign_value_tobasespace(columnIndex);
        
        tableData = get_param(table_handle, 'Table');
        assign_value_tobasespace(tableData);
    end
    
    %set nD RTWStorageClass
    all_nD_table_handles = find_system(sys_handle, 'FindAll', 'on', 'LookUnderMasks', 'all', 'BlockType', 'Lookup_n-D');
    table_num = length(all_nD_table_handles);
    for i = 1 : table_num
        table_handle = all_nD_table_handles(i);
        tableDimensions = get_param(table_handle, 'NumberOfTableDimensions');
        numOfTableDimensions = str2double(tableDimensions);
        for j = 1 : numOfTableDimensions
            breakPointForDimension = get_param(table_handle, ['BreakpointsForDimension', num2str(i)]);
            assign_value_tobasespace(breakPointForDimension);
        end
        
        tableData = get_param(table_handle, 'Table');
        assign_value_tobasespace(tableData);
    end
end


function tf = excludePortName(portName)
    tf = false;
    strArray = {'input', 'const', 'output', 'Max', 'In', 'Min', 'Out'};
    strLength = length(strArray);
    for i = 1 : strLength
        if strcmpi(portName, strArray{i})
            tf = true;
            break;
        end
    end
end
