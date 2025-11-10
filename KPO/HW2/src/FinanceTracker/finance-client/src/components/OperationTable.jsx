import React from "react";
import {
    Table, TableBody, TableCell, TableContainer, TableHead, TableRow, Paper
} from "@mui/material";

export default function OperationTable({ rows }) {
    return (
        <TableContainer component={Paper} variant="outlined">
            <Table size="small">
                <TableHead>
                    <TableRow>
                        <TableCell>Дата</TableCell>
                        <TableCell>Счёт</TableCell>
                        <TableCell>Категория</TableCell>
                        <TableCell align="right">Сумма</TableCell>
                        <TableCell>Описание</TableCell>
                    </TableRow>
                </TableHead>
                <TableBody>
                    {rows.map((o) => (
                        <TableRow key={o.id}>
                            <TableCell>{o.occurredOn}</TableCell>
                            <TableCell>{o.accountName ?? "-"}</TableCell>
                            <TableCell>{o.categoryName ?? "-"}</TableCell>
                            <TableCell
                                align="right"
                                style={{ color: o.amount >= 0 ? "green" : "crimson" }}
                            >
                                {o.amount.toFixed(2)}
                            </TableCell>
                            <TableCell>{o.description ?? "-"}</TableCell>
                        </TableRow>
                    ))}
                </TableBody>
            </Table>
        </TableContainer>
    );
}